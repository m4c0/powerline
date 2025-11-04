#pragma leco app
#pragma leco add_shader "poc.vert"
#pragma leco add_shader "poc.frag"

import casein;
import hai;
import vinyl;
import voo;

struct app_stuff {
  voo::device_and_queue dq { "poc-vinyl", casein::native_ptr };
  vee::render_pass rp = voo::single_att_render_pass(dq);
  voo::one_quad quad { dq };
};
static hai::uptr<app_stuff> gas {};

struct sized_stuff {
  voo::swapchain_and_stuff sw { gas->dq, *gas->rp };

  vee::pipeline_layout pl = vee::create_pipeline_layout();
  vee::gr_pipeline gp = vee::create_graphics_pipeline({
    .pipeline_layout = *pl,
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

static void frame() {
  if (!gss) gss.reset(new sized_stuff {});

  gss->sw.acquire_next_image();
  gss->sw.queue_one_time_submit(gas->dq.queue(), [&] {
    auto rp = gss->sw.cmd_render_pass({
      .command_buffer = gss->sw.command_buffer(),
      .clear_colours { vee::clear_colour(0, 0, 0, 1) },
    });
    auto cb = gss->sw.command_buffer();

    vee::cmd_bind_gr_pipeline(cb, *gss->gp);
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

  on(START,  [] { gas.reset(new app_stuff {}); });
  on(RESIZE, [] { gss.reset(nullptr); });
  on(FRAME,  &frame);
  on(STOP,   [] { 
    gss.reset(nullptr);
    gas.reset(nullptr);
  });

  return 0;
}();
