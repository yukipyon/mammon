#pragma once

#include <string>
#include <vector>

class chan_post {
    std::string board;
    std::string thread_id;
    std::string id;
    std::string sub;
    std::string com;
    std::vector<std::string> filenames;
    std::vector<std::string> filehashes;

public:
    /* Constructor for golden output unit tests. */
    chan_post(std::string board, std::string thread_id, std::string id, 
        std::string sub, std::string com, std::vector<std::string> filenames) :
            board(board), thread_id(thread_id), id(id), sub(sub), 
                com(com), filenames(filenames)
    {}

    /* Constructor for partially constructing a post and using it as a prototype
     * to base other posts on (i.e. members of the same thread. 
     */
    chan_post(std::string board, std::string thread_id) :
        board(board), thread_id(thread_id)
    {}

    chan_post finish(std::string id, std::string sub, 
        std::string com, std::vector<std::string> filenames) const;

    bool operator==(const chan_post &other) const;

    static std::string mk_filename(std::string tim, std::string ext);

    std::string get_board() const;
    std::string get_thread_id() const;
    std::string get_id() const;
    std::string get_sub() const;
    std::string get_com() const;
    std::vector<std::string> get_filenames() const;
	void print() const;
};

#include "kyukon/task.hpp"
struct thread {

    std::vector<chan_post> posts;
    task thread_task;
    int score;

    thread(const std::vector<chan_post> ps, const task t) :
        posts(ps), thread_task(t), score(0) {};

    bool operator<(const thread &other) const {
        return this->score < other.score;
    }
};
