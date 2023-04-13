#include "octree.h"

//contructor - building a tree
octree::octree(QPixmap pixmap, int N)
{
    int w = pixmap.width();
    int h = pixmap.height();
    QImage img = pixmap.toImage();
    this->root = NULL;
    this->leafCount = 0;
    this->maxLeaves = N;
    for(int x = 0; x < w; x++){
        for(int y = 0; y<h; y++){
            QColor pixel(img.pixel(x, y));
            this->add(pixel);
            while(leafCount > maxLeaves){
                this->reduce();
            }
        }
    }
}

//adding a new node
void octree::add(QColor pixel){
    if(this->root == NULL){
        this->root = createNode(0);
    }
    this->addRecursive(this->root, pixel, 0);
}

void octree::addRecursive(NODE* parent, QColor pixel, int depth){
    if(parent->isLeaf){
        parent->sumR += pixel.red();
        parent->sumG += pixel.green();
        parent->sumB += pixel.blue();
        parent->count += 1;
    }
    else{
        int i = this->getChildIndex(pixel, depth);
        if(parent->children[i] == NULL){
            parent->children[i] = createNode(depth + 1);
        }
        addRecursive(parent->children[i], pixel, depth + 1);
    }
}

//utulity function to get index of a pixel particular depth
int octree::getChildIndex(QColor pixel, int depth){
   int r = pixel.red() >> (7-depth) & 0x1;
   int g = pixel.green() >> (7-depth) & 0x1;
   int b = pixel.blue() >> (7-depth) & 0x1;
   return (r << 2) | (g << 1) | b;
}

//creating a node of a tree
NODE* octree::createNode(int depth){
    NODE* Node = new NODE();
    Node->sumR = 0;
    Node->sumG = 0;
    Node->sumB = 0;
    Node->isLeaf = (depth == 8);
    Node->count = 0;
    for(int i=0; i<8; i++) {
        Node->children[i] = NULL; //initialize all children
    }
    if(!(Node->isLeaf)){
        this->innerNodes[depth].push_back(Node); //add new inner node
    }
    else{
        this->leafCount += 1;
    }
    return Node;
}

//reduce - needed when the number of colors in pallete is to large
void octree::reduce(){
    int i = 7;
    while(i >= 0){
        if(!this->innerNodes[i].empty()){
            break;
        }
        i--;
    }
    NODE* Node = innerNodes[i].back();
    innerNodes[i].pop_back();
    int removed = 0;
    for(int k =0; k<=7; k++){
        if(!(Node->children[k] == NULL)){
            Node->sumR +=Node->children[k]->sumR;
            Node->sumG +=Node->children[k]->sumG;
            Node->sumB +=Node->children[k]->sumB;
            Node->count += Node->children[k]->count;
            delete(Node->children[k]);
            Node->children[k] = NULL;
            removed+=1;
        }
    }
    Node->isLeaf = true;
    this->leafCount +=(1 - removed);
}

//setting new pixel value from the tree
QPixmap octree::colorQuantization(QPixmap pixmap){
    int w = pixmap.width();
    int h = pixmap.height();
    QImage img = pixmap.toImage();
    for(int x = 0; x<w; x++){
        for(int y = 0; y<h; y++){
            QColor pixel = find(img.pixel(x, y));
            int r = pixel.red();
            int g = pixel.green();
            int b = pixel.blue();
            img.setPixel(x, y, qRgb(r, g, b));
        }
    }
    return QPixmap::fromImage(img);
}

QColor octree::find(QColor pixel){
    NODE *Node = this->root;
    int depth = 0;
    while(!Node->isLeaf){
        int i = getChildIndex(pixel, depth);
        Node = Node->children[i];
        depth += 1;
    }
    int r = Node->sumR / Node->count;
    int g = Node->sumG / Node->count;
    int b = Node->sumB / Node->count;
    return QColor(r,g,b);
}
