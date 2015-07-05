#ifndef HTML_PARSER
struct String{
  char* string;
  int length;
};

struct NodeTree {
  enum HTML_TAG** nodes;
  int size;
};


#endif
