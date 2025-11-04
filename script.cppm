#pragma leco add_resource script.lsp
export module script;
import jute;
import hai;
import lispy;
import silog;
import sires;

using namespace lispy;
using namespace lispy::experimental;

namespace script {
  hai::cstr source {};

  struct context : basic_context<node> {
    bool ended = false;
    int idx;
  };

  export int run(int idx) try {
    context ctx {
      .idx = idx,
    };
    ctx.fns["clear"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 0) err(n, "expecting no parameter");

      auto ctx = static_cast<context *>(n->ctx);
      if (ctx->ended) return n;

      silog::trace("clear");

      return n;
    };
    ctx.fns["title"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 3) err(n, "expecting x, y and text");

      auto ctx = static_cast<context *>(n->ctx);
      if (ctx->ended) return n;

      silog::trace("title");

      return n;
    };
    ctx.fns["text"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 3) err(n, "expecting x, y and text");

      auto ctx = static_cast<context *>(n->ctx);
      if (ctx->ended) return n;

      silog::trace("text");

      return n;
    };
    ctx.fns["pause"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 0) err(n, "expecting no parameter");

      auto ctx = static_cast<context *>(n->ctx);
      if (ctx->ended) return n;
      if (ctx->idx == 0) {
        silog::trace("ding");
        ctx->ended = true;
        return n;
      }

      ctx->idx--;

      return n;
    };
    ctx.run(source);

    return idx;
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
