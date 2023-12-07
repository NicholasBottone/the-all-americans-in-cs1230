#include "raytracer/raytracer.h"
#include <QThread>

/**
 * Extra credit -> own implementation of multithreading using QThreads.
 * NOT USED for illuminate (not any faster than QT's version), but was used in intersect.
 */

//struct intersectRoutineArgs {
//    RenderShapeData shape;
//    glm::vec4 pWorld;
//    glm::vec4 dWorld;
//};
//
//struct intersectData {
//    float distance;
//    glm::vec4 intersectionWorld;
//    glm::vec4 intersectionObj;
//    RenderShapeData intersectedShape;
//};
//
//Q_DECLARE_METATYPE(intersectData);
//
//class IntersectWorker : public QThread
//{
//    Q_OBJECT
//    void run() override {
//        exec();
//        /* ... here is the expensive or blocking operation ... */
//        glm::vec4 pObject = glm::inverse(a.shape.ctm) * a.pWorld;
//        glm::vec4 dObject = glm::normalize(glm::inverse(a.shape.ctm) * a.dWorld);
//
//        glm::vec4 intersectionObj = RayTracer::findIntersection(pObject, dObject, a.shape);
//        if (intersectionObj.w == 0) // no hit
//        {
//            const intersectData response{
//                    FINF,
//                    glm::vec4(0.f),
//                    glm::vec4(0.f),
//                    a.shape
//            };
//            ps.append(response);
//            emit data(response);
//        } else {
//            auto intersectionWorld = a.shape.ctm * intersectionObj;
//            float distance = glm::distance(intersectionWorld, a.pWorld);
//
//            const intersectData response{
//                    distance,
//                    intersectionWorld,
//                    intersectionObj,
//                    a.shape
//            };
//            ps.append(response);
//            emit data(response);
//        }
//        emit finished();
//    }
//public:
//    intersectRoutineArgs a;
//    QList<intersectData> &ps;
//    IntersectWorker(intersectRoutineArgs args, QList<intersectData> &p) : ps(p)
//    {
//        a = args;
//    }
//    signals:
//            void data(const intersectData &s);
//    void finished();
//};
//
//
//class IntersectController : public QObject
//{
//    Q_OBJECT
//public:
//    std::vector<QThread*> qthreads;
//    QList<intersectData> intersectPoints;
//    IntersectController(const std::vector<RenderShapeData> &shapes, glm::vec4 pWorld, glm::vec4 dWorld) {
//        qRegisterMetaType<const intersectData&>("myType");
//        int id = 0;
//        for (const RenderShapeData &shape: shapes) {
//            const intersectRoutineArgs threadArgs{shape, pWorld, dWorld};
//            IntersectWorker *thread = new IntersectWorker(threadArgs, intersectPoints);
//
//            connect(thread, &IntersectWorker::data, this, &IntersectController::addIntersectionPoint);
//            connect(thread, &IntersectWorker::finished, thread, &QThread::quit);
//
//            connect(thread, &IntersectWorker::finished, thread, &QThread::deleteLater);
//
//            qthreads.push_back(thread);
//        }
//    }
//    ~IntersectController() {
//        for (QThread* workerThread: qthreads) {
//            workerThread->exit();
//        }
//        qthreads.clear();
//        intersectPoints.clear();
//    }
//    void getClosestIntersection(float &minDist, glm::vec4 &closestIntersectionWorld, glm::vec4 &closestIntersectionObj, RenderShapeData intersectedShape) {
//        // start then wait
//        for (QThread* thread: qthreads) {
//            thread->start();
//        }
//        for (QThread* thread: qthreads) {
//            thread->quit();
//            thread->wait();
//        }
//
//
//        // once all threads are done, find the closest
//        for (const intersectData &i : intersectPoints) {
//            if (i.distance < minDist) {
//                minDist = i.distance;
//
//                intersectedShape = i.intersectedShape;
//                closestIntersectionObj = i.intersectionObj;
//                closestIntersectionWorld = i.intersectionWorld;
//            }
//        }
//}
//public slots:
//            void addIntersectionPoint(const intersectData &s) {
//        intersectPoints.append(s);
//    }
//    signals:
//            void operate(intersectRoutineArgs a);
//};
//
//#include "myqthreads.moc"