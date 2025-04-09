#pragma once

template <typename T>
struct MCTNode {
    using NodePtr=MCTNode<T>*;

    T data;
    NodePtr parent;
    NodePtr left;
    NodePtr right;
};