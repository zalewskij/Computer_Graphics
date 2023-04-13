
#include <vector>
#include <QPixmap>
#include <QImage>
#ifndef OCTREE_H
#define OCTREE_H



struct NODE{
    int sumR;
    int sumG;
    int sumB;
    int count;
    NODE* children[8];
    bool isLeaf;
};



class octree
{
private:
    //fields
    NODE *root;
    int leafCount;
    int maxLeaves;
    std::vector<NODE*> innerNodes[8];

    //methods
    NODE* createNode(int depth);
    void add(QColor pixel);
    void addRecursive(NODE *parent, QColor pixel, int depth);
    int getChildIndex(QColor color, int depth);
    void reduce();
    QColor find(QColor pixel);

public:
    //public methods
    octree(QPixmap pixmap, int N);
    QPixmap colorQuantization(QPixmap pixmap);
};

#endif // OCTREE_H
