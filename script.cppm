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

  struct context : basic_context<node> {
    hai::varray<natty::draw_params> nodes { 16 };
    bool ended = false;
    int index = 0;
    int rest;
  };

  export int run(natty::surface * surf, int idx) try {
    context ctx {
      .rest = idx,
    };
    ctx.fns["clear"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 0) err(n, "expecting no parameter");

      auto ctx = static_cast<context *>(n->ctx);
      if (ctx->ended) return n;

      ctx->nodes.truncate(0);

      return n;
    };
    ctx.fns["title"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 3) err(n, "expecting x, y and text");
      if (!is_atom(aa[2])) err(aa[2], "expecting an atom");

      auto x = to_i(aa[0]);
      auto y = to_i(aa[1]);

      auto ctx = static_cast<context *>(n->ctx);
      if (ctx->ended) return n;

      ctx->nodes.push_back_doubling(natty::draw_params {
        .font = *font_title,
        .position { x, y },
        .text = aa[2]->atom,
      });

      return n;
    };
    ctx.fns["text"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 3) err(n, "expecting x, y and text");
      if (!is_atom(aa[2])) err(aa[2], "expecting an atom");

      auto x = to_i(aa[0]);
      auto y = to_i(aa[1]);

      auto ctx = static_cast<context *>(n->ctx);
      if (ctx->ended) return n;

      ctx->nodes.push_back_doubling(natty::draw_params {
        .font = *font,
        .position { x, y },
        .text = aa[2]->atom,
      });

      return n;
    };
    ctx.fns["pause"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 0) err(n, "expecting no parameter");

      auto ctx = static_cast<context *>(n->ctx);
      if (ctx->ended) return n;
      if (ctx->rest <= 0) {
        ctx->ended = true;
        return n;
      }

      ctx->rest--;
      ctx->index++;

      return n;
    };
    ctx.run(source);

    for (auto p: ctx.nodes) {
      p.surface = surf;
      natty::draw(p);
    }

    return ctx.index;
  } catch (const parser_error & e) {
    silog::log(silog::error, "%s", to_file_err("script.lsp", e).begin());
    return idx;
  }

  export void load(auto && cb) {
    sires::read("script.lsp", nullptr, [cb](void *, hai::cstr & src) {
      source = traits::move(src);
      cb();
    });
  }
}
