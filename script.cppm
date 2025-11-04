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

  void run() try {
    silog::log(silog::info, "Processing script");
  } catch (const parser_error & e) {
    silog::log(silog::error, "%s", to_file_err("script.lsp", e).begin());
  }

  export void load(auto && cb) {
    sires::read("script.lsp", nullptr, [cb](void *, hai::cstr & src) {
      source = traits::move(src);
      cb();
    });
  }
}
