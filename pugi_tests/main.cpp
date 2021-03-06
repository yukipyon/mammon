#define PUGIXML_HEADER_ONLY
#include "pugi_lib/pugixml.cpp"
#include <iostream>

int main() {

	const char source[] = "<mesh name='sphere'><bounds>0 0 1 1</bounds></mesh>";
	size_t size = sizeof(source);

	char *buffer = new char[size];
	memcpy(buffer, source, size);

	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_buffer_inplace_own(buffer, size);

	pugi::xpath_node_set bounds = doc.select_nodes("/mesh/bounds");

	std::cout << "Bounds:";

	for (const auto &node : bounds) {
	
	        std::cout << " " << node.node().name();
	}

	std::cout << "\n\nhello world" << std::endl;
}
