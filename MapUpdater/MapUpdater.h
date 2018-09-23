#ifndef MAPUPDATER_H
#define MAPUPDATER_H

#include <QObject>
#include <QGeoCoordinate>
#include <thread>
#include "EKF.h"
#include "KittiDatasetTools/KittiDatasetParser.h"

class MapUpdater : public QObject
{
    Q_OBJECT
public:
    explicit MapUpdater(QObject *parent = nullptr);
    ~MapUpdater();

private:
    kitti_dataset_tools::KittiDatasetParser *parser_;
    EKF_INS::EKF *ekf_;
    QObject *load_button_, *start_button_, *osm_map_, *map_circle_start_, *map_circle_route_, *folder_path_;
    bool is_load_button_clicked_ = false;
    bool is_start_button_clicked_ = false;
    Eigen::Vector3d position_state_;
    std::thread *playing_thread_;

private slots:
    void slotLoadButtonClicked();
    void slotStartButtonClicked();
    void slotUpdateMapRoute();
};

#endif // MAPUPDATER_H
