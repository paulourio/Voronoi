#include <cstddef>

#include <glog/logging.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <dcel/dcel.hh>
#include <dcel/Vertex.hh>
#include <voronoi/geometry.hh>

using voronoi::Point;
using voronoi::FaceInfo;

typedef dcel::DCEL<Point*, Point*, FaceInfo*> MyDCEL;

static MyDCEL::Vertex* createVertex(MyDCEL& d, double x, double y)
{
	MyDCEL::Vertex *v = d.createGetVertex();
	v->getData() = new voronoi::Point(x, y);
	EXPECT_TRUE(v != NULL) << "Erro ao criar vértice";
	return v;
}

static void createEdge(MyDCEL& d, MyDCEL::Vertex* origin, MyDCEL::Face* face,
		MyDCEL::Vertex* twinOrigin, MyDCEL::Face* twinFace,
		MyDCEL::HalfEdge** outHalfEdge, MyDCEL::HalfEdge** outTwinHalfEdge)
{
	unsigned int i = d.createEdge(origin, face, twinOrigin, twinFace);

	*outHalfEdge = d.getHalfEdge(i);
	EXPECT_TRUE(*outHalfEdge != NULL) << "Erro ao criar meia aresta";
	*outTwinHalfEdge = d.getHalfEdge(i + 1);

	origin->setIncidentEdge(*outHalfEdge);
	twinOrigin->setIncidentEdge(*outTwinHalfEdge);
}


TEST(DCELCheckTest, UnitTestingFrameworkWorks)
{
	LOG(INFO) << "Starting DCEL check test.";
	const size_t VERTICES = 4;
	const size_t EDGES = 10;
	const size_t FACES = 2;

	MyDCEL d(VERTICES, EDGES, FACES);

	EXPECT_TRUE(d.getVertices().capacity() == VERTICES) << "Memória não alocada para os vértices.";
	EXPECT_TRUE(d.getHalfEdges().capacity() == EDGES * 2) << "Memória não alocada para as meia arestas.";
	EXPECT_TRUE(d.getFaces().capacity() == FACES) << "Memória não alocada para as faces.";

	MyDCEL::Vertex* v1 = createVertex(d, 0, 4);
	MyDCEL::Vertex* v2 = createVertex(d, 2, 4);
	MyDCEL::Vertex* v3 = createVertex(d, 2, 2);
	MyDCEL::Vertex* v4 = createVertex(d, 1, 1);

	MyDCEL::Face* f1 = d.createGetFace(NULL);
	MyDCEL::Face* f2 = d.createGetFace(NULL);

	f1->getData() = new FaceInfo(1);
	f2->getData() = new FaceInfo(2);

	EXPECT_TRUE(f1 != NULL) << "Face 1 não criada";
	EXPECT_TRUE(f2 != NULL) << "Face 2 não criada";

	MyDCEL::HalfEdge *e11, *e12, *e21, *e22, *e31, *e32, *e41, *e42;

	e11 = e12 = e21 = e22 = e31 = e32 = e41 = e42 = NULL;

	/*  		  Src 	Face 	TwinSrc	TwinFace	HalfEdge	TwinHalfEdge */
	createEdge(d, v1,	f1, 	v2, 	f2, 		&e11, 		&e12);
	createEdge(d, v3,	f1, 	v4, 	f1, 		&e21, 		&e22);
	createEdge(d, v3, 	f1, 	v1, 	f2, 		&e31, 		&e32);
	createEdge(d, v3, 	f2, 	v2, 	f1, 		&e41, 		&e42);

	e11->setNext(e42);
	e12->setNext(e32);
	e21->setNext(e22);
	e22->setNext(e31);
	e31->setNext(e11);
	e32->setNext(e41);
	e41->setNext(e12);
	e42->setNext(e21);

	f1->setBoundary(e22);
	f2->setBoundary(e32);

	d.checkAllFaces();

	delete f1->getData();
	delete f2->getData();

	delete v1->getData();
	delete v2->getData();
	delete v3->getData();
	delete v4->getData();
	d.clear();

	//d.getFace(0)->getData() = 0;

	LOG(INFO) << "Finishing DCEL check test.";
}

