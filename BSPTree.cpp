#include "BSPTree.h"

/********************************************************************************************
 *                                FUNCIONES DE INSERCIÓN
 ********************************************************************************************/
void BSPNode::insert(const LineSegment &segment)
{
    segments.push_back(segment);
}

BSPNode *BSPNode::insert(const std::vector<LineSegment> &inputSegments)
{
    // Aqui su codigo
    for (const auto &segment : inputSegments)
    {
        insert(segment);
    }
    return this;
}

void BSPTree::insert(const std::vector<LineSegment> &segments)
{
    // Aqui su codigo
    if (!root)
    {
        root = new BSPNode(segments);
    }
    else
    {
        // Inserta los segmentos en el nodo raíz
        root->insert(segments);
    }
}

/********************************************************************************************
 *                                FUNCIONES DE COLLISSION
 ********************************************************************************************/
CollisionResult BSPTree::checkCollision(const Point &initialPoint, const Point &finalPoint, const Point &Player) const
{
    // Aqui escriba su codigo
}

/********************************************************************************************
 *                                FUNCIONES DE IMPRESIÓN
 ********************************************************************************************/
void BSPNode::print(BSPNode *node, int depth)
{
    if (node == nullptr)
        return;

    for (int i = 0; i < depth; ++i)
        std::cout << "  ";
    std::cout << "Node: " << node->getLine() << " (Leaf: " << node->isLeaf() << ")\n";

    auto segments = node->getSegments();
    for (const auto &segment : segments)
    {
        for (int i = 0; i < depth; ++i)
            std::cout << "  ";
        std::cout << "  Segment: " << segment << "\n";
    }

    print(node->getFrontNode(), depth + 1);
    print(node->getBackNode(), depth + 1);
}

void BSPTree::print()
{
    root->print(root);
}

/********************************************************************************************
 *                                FUNCIONES DE TESTEO
 ********************************************************************************************/
bool BSPNode::verifySegment(const LineSegment &segment) const
{
    // Verificar si el segmento está en la lista de segmentos de este nodo (hoja o interno)
    if (std::find(segments.begin(), segments.end(), segment) != segments.end())
    {
        return true;
    }

    // Si el nodo es una hoja y el segmento no se encontró, entonces retornar false
    if (isLeaf())
    {
        return false;
    }

    // Si el nodo es interno, verificar en los nodos hijos
    LineSegment front, back;
    RelationType relation = segment.relationToLine(line, front, back);

    switch (relation)
    {
    case COINCIDENT:
        return true; // Ya verificamos esto al inicio
    case IN_FRONT:
        return frontNode ? frontNode->verifySegment(segment) : false;
    case BEHIND:
        return backNode ? backNode->verifySegment(segment) : false;
    case SPANNING:
        return (frontNode ? frontNode->verifySegment(front) : false) &&
               (backNode ? backNode->verifySegment(back) : false);
    }
    return false; // No debería llegar aquí
}

void BSPTree::test()
{
    std::vector<LineSegment> allSegments = {
        LineSegment(Point(3, 6), Point(7, 6)),
        LineSegment(Point(3, 4), Point(7, 4)),
        LineSegment(Point(1, 3), Point(1, 7)),
        LineSegment(Point(9, 3), Point(9, 7)),
        LineSegment(Point(5, 1), Point(5, 3)),
    };
    insert(allSegments);
    // print();
    std::cout << "Verificando segmentos:\n";
    for (const auto &segment : allSegments)
    {
        std::cout << "  " << segment << ": " << (root->verifySegment(segment) ? "true" : "false") << "\n";
    }
}