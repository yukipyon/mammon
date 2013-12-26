#define PUGIXML_HEADER_ONLY
#include "../pugi_lib/pugixml.cpp"
#include "../base_parser.hpp"
#include <iostream>
#include "chan_parser.hpp"
#include "chan_post.hpp"

#include <fstream>

std::vector<chan_post> chan_parser::parse_a_thread(const char *board, 
	const pugi::xml_node &node) 
{

	//Parse the op.
	chan_post thread = parse_post(board, node);
	std::string thread_id = thread.post_id;

	//Parse each reply (not OP)
	auto nodes = node.select_nodes("table/tbody/tr/td");
	auto replies = parse_posts(board, thread_id, std::move(nodes));

	//TODO This is horrible.
	replies.insert(replies.begin(), thread);

	return std::move(replies);
}
 
//Given a list page, return a list of thread previews i.e.
//op + the last few replies.
std::vector<std::vector<chan_post>> chan_parser::parse_threads(
	const char *board,
	const std::string &xml)
{

	const char *xpath = "//form/div[@id and not(@style)]";

	pugi::xml_document doc;
	doc.load(xml.c_str());
	
	//Parse each thread <div>
	auto ops = doc.select_nodes(xpath);

	std::vector<std::vector<chan_post>> threads;

	//For each thread...
	for (auto op : ops)
		threads.push_back(parse_a_thread(board, op.node()));

	return threads;
}

//List page.
std::vector<std::string> chan_parser::parse_thread_ids() {

	const char *filename = "input.html";
	const char *xpath = "//form/div[@id and not(@style)]";
	const char *att = "id";

	std::vector<std::string> ids = base_parser::parse_path(filename, xpath, att);
	std::vector<std::string> ret;

	//Only take the odd numbered values.
	for (unsigned i = 1; i < ids.size(); i+=2) {
		ret.push_back(ids[i]);
	}

	std::ifstream ifs;
	ifs.open("input.html");
	std::string xml, tmp;

	while(std::getline(ifs, tmp))
		xml += tmp;

	auto threads = parse_threads("test", xml);

	for (const auto &thread: threads) {

		for (const auto &post : thread) {

			std::cout << "##############################" << std::endl;
			std::cout << "Board = " << post.board << std::endl;
			std::cout << "Thread = " << post.thread_id << std::endl;
			std::cout << "Id = " << post.post_id << std::endl;
			std::cout << "Image = " << post.img_url << std::endl;
			std::cout << "Image = " << post.img<< std::endl;
			std::cout << "Text = " << post.content << std::endl;
			std::cout << "##############################" << std::endl;
		}

		std::cout << "##############################" << std::endl;
		std::cout << "##############################" << std::endl;
		std::cout << "##############################" << std::endl;
		std::cout << "##############################" << std::endl;
	}

	return ret;
}


//Post page
std::string chan_parser::parse_postid(const pugi::xml_node &node) {

	return base_parser::parse_first_path(node, "a[@name]", "name");
}

std::string chan_parser::parse_post_text(const pugi::xml_node &node) {

	pugi::xpath_node quote = node.select_single_node("blockquote");

	//Flatten the subtree into a single string.
	return flatten(quote.node());
}

std::string chan_parser::parse_post_img(const pugi::xml_node &node) {

	return base_parser::parse_first_path(node, "span[@class='filesize']/a", "href");
}

std::string chan_parser::parse_post_img_name(const pugi::xml_node &node) {

	pugi::xpath_node img = node.select_single_node("span[@class='filesize']/a");
	return img.node().child_value();
}

//Given a subtree will return a vector of posts.
std::vector<chan_post> chan_parser::parse_posts(const char *board, 
	const std::string &thread_id, pugi::xpath_node_set &&posts) 
{

	std::vector<chan_post> ret;

	for (const auto &node : posts) {

		chan_post cp = parse_post(board, node.node(), thread_id);

		if (!cp.post_id.empty())
			ret.push_back(std::move(cp));
	}

	return std::move(ret);
}

//Returns an empty object if it cannot be parsed.
chan_post chan_parser::parse_post(const char *board, const pugi::xml_node &node, 
	const std::string &thread_id) 
{

	std::string post_id = parse_postid(node);

	if (post_id.empty())
		return chan_post();

	std::string content = parse_post_text(node);
	std::string img_src = parse_post_img(node);
	std::string img_name = parse_post_img_name(node);

	const std::string &thread = thread_id.empty()? post_id : thread_id;

	return chan_post(board, thread, std::move(post_id), 
			std::move(img_name), std::move(img_src), std::move(content));
}

std::vector<chan_post> chan_parser::parse_thread(const std::string &xml) {

	//Parse file into an AST.
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load(xml.c_str());

	if (!result)
		std::cout << "Error parsing document: " << result.description() << std::endl;

	//Parse the original post subtree.
	pugi::xpath_node_set op = doc.select_nodes("//form/div[2]");

	std::vector<chan_post> thread = parse_posts("test", "", std::move(op));

	std::string op_postid = thread.front().thread_id;

	//Parse the thread responses into a node set.
	op = doc.select_nodes("//table/tbody/tr/td");

	std::vector<chan_post> replies = parse_posts("test", op_postid, std::move(op));
	
	//TODO is there a way to move this?
	thread.insert(thread.end(), replies.begin(), replies.end());

	return std::move(thread);
}

//Pugi XML developers have been writing too much Java...
struct my_walker : public pugi::xml_tree_walker {

	std::string value;
	bool for_each(pugi::xml_node &node) {

		std::string tmp(node.value());

		if (!tmp.empty()) {

			if (!value.empty())
				value += " ";

			value += tmp;
		}

		return true;
	}
};

std::string chan_parser::flatten(pugi::xml_node &&quote) {

	my_walker k;
	quote.traverse(k);
	return std::move(k.value);
}

