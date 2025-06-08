
#ifndef UNTITLED5_BASE_H
#define UNTITLED5_BASE_H

#include <QGraphicsPixmapItem>
class Base : public QGraphicsPixmapItem {
public:
    Base(qreal x, qreal y);
    void setTeam(int team);
    int getTeam();
    void setId(int id);
    int getId() const;
private:
    int team=-1;
    int id;
};



#endif //UNTITLED5_BASE_H
