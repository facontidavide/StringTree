#define CATCH_CONFIG_MAIN
#include "catch2.hpp"
#include <iostream>
#include "stringtree.h"

TEST_CASE( "Basic Testing", "StringNode" )
{
    StringNode root("ROOT", nullptr);

    auto one = root.addChild("One");
    auto two = root.addChild("Two");

    auto node_A = one->addChild("nodeA");
    auto node_B = one->addChild("nodeB");

    auto node_X = two->addChild("nodeX");
    auto node_Y = two->addChild("nodeY");

    std::cout << node_Y->toString() << std::endl;

    REQUIRE( node_A->toString() == "ROOT/One/nodeA" );
    REQUIRE( node_B->toString() == "ROOT/One/nodeB" );
    REQUIRE( node_X->toString() == "ROOT/Two/nodeX" );
    REQUIRE( node_Y->toString() == "ROOT/Two/nodeY" );
    //---------------------------------
    // Test equal operator

    StringNode _root("ROOT", nullptr);
    auto _one    = _root.addChild("One");
    auto _node_A = _one->addChild("nodeA");

    REQUIRE( root == _root ); // same branch/hash
    REQUIRE( *one == *_one ); // same branch/hash
    REQUIRE( *node_A == *_node_A ); // same branch/hash

    std::cout << PrintTree(&root) << std::endl << std::endl;

    //--------------------------------
    // Test CloneBranch
    REQUIRE( CloneBranch( node_Y, &_root) );

    REQUIRE( _root.children().count("Two") );
    auto _two = _root.children().at("Two");
    REQUIRE( _two->children().count("nodeY") );

    std::cout << PrintTree(&_root) << std::endl;

}

