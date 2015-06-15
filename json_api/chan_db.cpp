#include "chan_db.hpp"
#include "chan_post.hpp"
#include "../base_db.hpp"

chan_db::chan_db(const std::string &chan_name)
    : cname(chan_name)
{
    p_table = chan_name + "_posts";
    f_table = chan_name + "_files";

    base_db::init(chan_name + ".sqlite");
    base_db::init_table(p_table, {
        "board",
        "thread_id",
        "id",
        "sub",
        "com"});

    base_db::format_table(f_table, {
        "id",
        "filename"});
}

bool chan_db::store_post(const chan_post &cp)
{
    if (post_exists(cp))
        return true;

	base_db::insert_row(p_table, {
        cp.get_board(), 
        cp.get_thread_id(), 
        cp.get_id(), 
        cp.get_sub(), 
        cp.get_com()});

    for (const auto &fn : cp.get_filenames())
        base_db::insert_row(f_table, {cp.get_id(), fn});

    return true;
}

bool chan_db::post_exists(const chan_post &cp)
{
    const std::string query = 
        "select * from " + p_table + " where board=? and id=?";

    const std::string res = base_db::lookup_single_value(
        query.c_str(), std::vector<std::string>({cp.get_board(), cp.get_id()}));

    return !res.empty();
}