#include "fourchan_proc.hpp"

#include "eightchan_proc.hpp"

#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <map>

#include <chibi/eval.h>
#include "score.hpp"
#include "ext.hpp"

int main()
{
    sexp ctx = sexp_make_eval_context(NULL, NULL, NULL, 0, 0);
    sexp_load_standard_env(ctx, NULL, SEXP_SEVEN);

    sexp_gc_var1(score);
    sexp_gc_preserve1(ctx, score);

    bool ret = ext::load_file(ctx, "mammon.ss");
    if (!ret) {
        std::cout << "Could not find configuration file `mammon.ss`."
            << std::endl;
        
        return 1;
    }

    score = sexp_eval_string(ctx, "score", -1, NULL);
    if (sexp_procedurep(score)) {
        score::ext_score = [&ctx](const char *b, const char *t) {

            return ext::call_fun_str2(ctx, "score", b, t);
        };
    }

    const auto boards = ext::config_get_list(ctx, "boards");
    if (boards.empty()) {
        std::cout << "Please add a value for `boards` in your configuration "
            "file.\ne.g. (define boards '(\"g\" \"a\" \"k\"))" << std::endl;

        return 1;
    }

    const std::string dom = ext::config_get_str(ctx, "dom", "");
    std::unique_ptr<chan_proc> pc;
    if (dom == "4chan") {
        pc = std::unique_ptr<chan_proc>(new fourchan_proc());
    } else if (dom == "8chan") {
        std::string url = ext::config_get_str(ctx, "url-8chan", "");
        pc = std::unique_ptr<chan_proc>(new eightchan_proc(std::move(url)));
    } else {
        std::cout << "Please specify a value for `dom` in your configuration "
            "file.\nPossible values are:" << std::endl;
        std::cout
            << "\t4chan\n"
            << "\t8chan\n"
            << std::endl;

        return 1;
    }

    for (const auto &b: boards)
        pc->proc_board(b);

    sexp_gc_release1(ctx);
    sexp_destroy_context(ctx);
    return 0;
}
