#pragma leco app
#pragma leco add_shader "poc.vert"
#pragma leco add_shader "poc.frag"

import casein;
import hai;
import natty;
import vinyl;
import voo;

struct app_stuff {
  voo::device_and_queue dq { "poc-vinyl", casein::native_ptr };
  vee::render_pass rp = voo::single_att_render_pass(dq);
  voo::one_quad quad { dq };

  voo::single_frag_dset text_dset { 1 };
  vee::pipeline_layout pl = vee::create_pipeline_layout(
    text_dset.descriptor_set_layout()
  );

  vee::sampler text_smp = vee::create_sampler(vee::linear_sampler);
  voo::h2l_image text_img {{
    .pd = dq.physical_device(),
    .w = 1024,
    .h = 1024,
    .fmt = VK_FORMAT_R8G8B8A8_SRGB,
  }}; 
  bool text_loaded = false;

  natty::font_t text_font = natty::create_font("Times", 48);
  natty::surface_t text_surf = natty::create_surface(text_img.width(), text_img.height());
};
static hai::uptr<app_stuff> gas {};

struct sized_stuff {
  voo::swapchain_and_stuff sw { gas->dq, *gas->rp };

  vee::gr_pipeline gp = vee::create_graphics_pipeline({
    .pipeline_layout = *gas->pl,
    .render_pass = *gas->rp,
    .extent = sw.extent(),
    .shaders {
      voo::shader { "poc.vert.spv" }.pipeline_vert_stage(),
      voo::shader { "poc.frag.spv" }.pipeline_frag_stage(),
    },
    .bindings {
      voo::one_quad::vertex_input_bind(),
    },
    .attributes {
      voo::one_quad::vertex_attribute(0),
    },
  });
};
static hai::uptr<sized_stuff> gss {};

static void start() {
  gas.reset(new app_stuff {});

  vee::update_descriptor_set(gas->text_dset.descriptor_set(), 0, gas->text_img.iv(), *gas->text_smp);
}

static void frame() {
  if (!gss) gss.reset(new sized_stuff {});

  gss->sw.acquire_next_image();
  gss->sw.queue_one_time_submit(gas->dq.queue(), [&] {
    if (!gas->text_loaded) {
      {
        natty::draw({
          .surface = gas->text_surf,
          .font = gas->text_font,
          .position { 0, 0 },
          .text { "Olá bravo novo mundo!" },
        });

        voo::memiter<unsigned> pixies { gas->text_img.host_memory() };
        auto ptr = natty::surface_data(*gas->text_surf).begin();
        for (auto i = 0; i < gas->text_img.width() * gas->text_img.height(); i++) {
          pixies += *ptr++;
        }
      }
      gas->text_img.setup_copy(gss->sw.command_buffer());
      gas->text_loaded = true;
    }

    auto rp = gss->sw.cmd_render_pass({
      .command_buffer = gss->sw.command_buffer(),
      .clear_colours { vee::clear_colour(0, 0, 0, 1) },
    });
    auto cb = gss->sw.command_buffer();

    vee::cmd_bind_gr_pipeline(cb, *gss->gp);
    vee::cmd_bind_descriptor_set(cb, *gas->pl, 0, gas->text_dset.descriptor_set());
    gas->quad.run(cb, 0);
  });
  gss->sw.queue_present(gas->dq.queue());
}

const auto i = [] {
  using namespace casein;
  using namespace vinyl;

  handle(KEY_DOWN, K_F, [] {
    fullscreen = !fullscreen;
    interrupt(IRQ_FULLSCREEN);
  });

  on(START,  &start);
  on(RESIZE, [] { gss.reset(nullptr); });
  on(FRAME,  &frame);
  on(STOP,   [] { 
    gss.reset(nullptr);
    gas.reset(nullptr);
  });

  return 0;
}();
