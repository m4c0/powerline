#pragma leco add_resource script.lsp
export module script;
import jute;
import hai;
import lispy;
import natty;
import silog;
import sires;

using namespace lispy;
using namespace lispy::experimental;

namespace script {
  hai::cstr source {};

  natty::font_t font_title = natty::create_font("Cascadia Mono", 96);
  natty::font_t font = natty::create_font("Times", 72);

  struct data {
    hai::varray<natty::draw_params> nodes { 16 };
    bool ended = false;
    int index = 0;
    int rest;
  };

  export int run(natty::surface * surf, int idx) try {
    data d { .rest = idx };

    temp_frame ctx {};
    ctx.ptrs["data"] = &d;
    ctx.fns["clear"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 0) erred(n, "expecting no parameter");

      auto ctx = static_cast<data *>(context()->ptr("data"));
      if (ctx->ended) return n;

      ctx->nodes.truncate(0);

      return n;
    };
    ctx.fns["title"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 3) erred(n, "expecting x, y and text");
      if (!is_atom(aa[2])) erred(aa[2], "expecting an atom");

      auto x = to_i(aa[0]);
      auto y = to_i(aa[1]);

      auto ctx = static_cast<data *>(context()->ptr("data"));
      if (ctx->ended) return n;

      ctx->nodes.push_back_doubling(natty::draw_params {
        .font = *font_title,
        .position { x, y },
        .text = aa[2]->atom,
      });

      return n;
    };
    ctx.fns["text"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 3) erred(n, "expecting x, y and text");
      if (!is_atom(aa[2])) erred(aa[2], "expecting an atom");

      auto x = to_i(aa[0]);
      auto y = to_i(aa[1]);

      auto ctx = static_cast<data *>(context()->ptr("data"));
      if (ctx->ended) return n;

      ctx->nodes.push_back_doubling(natty::draw_params {
        .font = *font,
        .position { x, y },
        .text = aa[2]->atom,
      });

      return n;
    };
    ctx.fns["pause"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 0) erred(n, "expecting no parameter");

      auto ctx = static_cast<data *>(context()->ptr("data"));
      if (ctx->ended) return n;
      if (ctx->rest <= 0) {
        ctx->ended = true;
        return n;
      }

      ctx->rest--;
      ctx->index++;

      return n;
    };

    temp_arena<node> mem {};
    lispy::run<node>("script.lsp", source);

    natty::clear(surf);
    for (auto p: d.nodes) {
      p.surface = surf;
      natty::draw(p);
    }

    return d.index;
  } catch (const parser_error & e) {
    silog::log(silog::error, "%s", to_file_err(e).begin());
    return idx;
  }

  export void load(auto && cb) {
    sires::read("script.lsp", nullptr, [cb](void *, hai::cstr & src) {
      source = traits::move(src);
      cb();
    });
  }
}
