#ifndef UNMUTABLE_STRINGTREE_H
#define UNMUTABLE_STRINGTREE_H

#include <string>
#include <unordered_map>
#include <iostream>
#include <functional>

class StringNode
{

public:

    explicit StringNode(const std::string& value, const StringNode* parent);

    ~StringNode();

    const StringNode* parent() const     { return _parent; }

    const std::string& value() const     { return _value; }

    StringNode *addChild(const std::string& child );

    bool isLeaf() const { return _children.empty(); }

    bool isRoot() const { return _parent  == nullptr; }

    std::string toString(char delimiter = '/') const;

    size_t hash() const { return _hash; }

    bool operator == (const StringNode& other) const
    {
        return this->hash() == other.hash();
    }
    bool operator != (const StringNode& other) const
    {
        return !( *this == other );
    }

    typedef std::unordered_map<std::string, StringNode*> ChildrenMap;

    const ChildrenMap& children() const  { return _children; }

private:
    const StringNode* _parent;
    std::string       _value;
    ChildrenMap       _children;
    size_t            _hash;
};

//-----------------------------------------

inline StringNode::StringNode( const std::string &value, const StringNode *parent):
    _parent(parent),
    _value(value)
{
    size_t h1 = (_parent ? _parent->hash() : 0 );
    size_t h2 = std::hash<std::string>()(value);
    _hash = h1  ^ ( h2 << 1);
}

inline StringNode::~StringNode()
{
    for( auto& it: _children)
    {
        delete it.second;
    }
}

inline StringNode *StringNode::addChild(const std::string& value)
{
    auto it = _children.find(value);
    if( it == _children.end() )
    {
        auto new_node = new StringNode( value, this );
        it = _children.insert( { value, new_node } ).first;
    }
    return (it->second);
}

inline std::string StringNode::toString(char delimiter) const
{
    std::string out;
    const std::string* str_array[16];
    int index = 0;
    size_t out_size = 0;
    const StringNode* node = this;
    while(node != nullptr)
    {
        str_array[index++] = &node->value();
        out_size += node->value().size();
        node = node->parent();
    }
    const size_t array_size = static_cast<size_t>(index);
    index--;
    out.reserve(out_size + array_size);
    while( index >= 0)
    {
        out.append( *str_array[index] );
        if( index != 0)
        {
            out.push_back( delimiter );
        }
        index--;
    }
    return out;
}

inline std::string PrintTree( const StringNode* node, int indent = 0)
{
    std::string out;
    out.reserve(128);

    for (int i=0; i<indent; i++)
    {
        out.push_back('.');
    }
    out.append( node->value()).push_back('\n');

    for (const auto& child: node->children() )
    {
        out += PrintTree( child.second, indent+3);
    }
    return out;
}


#endif // UNMUTABLE_STRINGTREE_H

