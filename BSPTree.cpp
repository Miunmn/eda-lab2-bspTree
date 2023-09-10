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

void BSPTree::testCollisionDetection()
{
    std::cout << "Iniciando pruebas de detección de colisiones..." << std::endl;

    // Crear un árbol BSP
    BSPTree tree;
    std::vector<LineSegment> segments = {
        LineSegment(Point(50, 50), Point(550, 50)),
        LineSegment(Point(550, 50), Point(550, 550)),
        LineSegment(Point(550, 550), Point(50, 550)),
        LineSegment(Point(50, 550), Point(50, 50))};

    std::vector<std::pair<Point, Point>> testCases = {
        {Point(293.678, 51.4665), Point(293.526, 0.6811)},
        {Point(293.526, 50.6811), Point(293.374, 9.8958)},
        {Point(293.374, 49.8958), Point(293.221, 9.1104)}};

    // Insertar los segmentos en el árbol
    tree.insert(segments);
    tree.print();
    std::cout << std::endl;

    for (const auto &testCase : testCases)
    {
        Point initial = testCase.first;
        Point final = testCase.second;

        CollisionResult result = tree.checkCollision(initial, final, Point(0, 0));
        std::cout << "Prueba desde " << initial << " hasta " << final << ": ";

        if (result.collisionDetected)
        {
            std::cout << "Colisión detectada con vector normal " << result.normal << std::endl;
        }
        else
        {
            std::cout << "No se detectó colisión" << std::endl;
        }
    }
}