#include "MapUpdater.h"
#include <QDebug>

MapUpdater::MapUpdater(QObject *parent) : QObject(parent)
{
    load_button_ = parent->findChild<QObject*>("loadButton");
    if (!load_button_) {
        qDebug() << "Can't locate loadButton";
        return;
    }
    start_button_ = parent->findChild<QObject*>("startButton");
    if (!start_button_) {
        qDebug() << "Can't locate startButton";
        return;
    }
    osm_map_ = parent->findChild<QObject*>("osmMap");
    if (!osm_map_) {
        qDebug() << "Can't locate osmMap";
        return;
    }
    map_circle_start_ = parent->findChild<QObject*>("mapCircleStart");
    if (!map_circle_start_) {
        qDebug() << "Can't locate mapCircleStart";
        return;
    }
    map_circle_route_ = parent->findChild<QObject*>("mapCircleRoute");
    if (!map_circle_route_) {
        qDebug() << "Can't locate mapCircleRoute";
        return;
    }
    folder_path_ = parent->findChild<QObject*>("folderPath");
    if (!folder_path_) {
        qDebug() << "Can't locate folderPath";
        return;
    }
    velocity_text_ = parent->findChild<QObject*>("velocityText");
    if (!velocity_text_) {
        qDebug() << "Can't locate velocityText";
        return;
    }
    azimuth_text_ = parent->findChild<QObject*>("azimuthText");
    if (!azimuth_text_) {
        qDebug() << "Can't locate azimuthText";
        return;
    }

    ekf_ = new EKF_INS::EKF();
    parser_ = new kitti_dataset_tools::KittiDatasetParser(this);

    connect(load_button_, SIGNAL(clicked()), this, SLOT(slotLoadButtonClicked()));
    connect(start_button_, SIGNAL(clicked()), this, SLOT(slotStartButtonClicked()));
    connect(parser_, SIGNAL(newDataIsReady()), this, SLOT(slotUpdateMapRoute()));

    km_h_str_ = " [Km/h]";
    degree_str_ = " [°]";
}

MapUpdater::~MapUpdater() {
    playing_thread_->join();
    delete parser_;
    delete ekf_;
}

void MapUpdater::slotLoadButtonClicked() {
    if (!is_load_button_clicked_) {
        is_load_button_clicked_ = true;
        QString path_to_oxts = folder_path_->property("text").toString().remove(0,7);
        parser_->setParserSettings(path_to_oxts.toUtf8().constData(), *ekf_);
        parser_->loadDataset();
        parser_->initState();
        position_state_ = ekf_->getPositionState();
        QGeoCoordinate initial_coordinate(EKF_INS::Utils::radianToDegree(position_state_(0)),EKF_INS::Utils::radianToDegree(position_state_(1)));
        osm_map_->setProperty("center",QVariant::fromValue(initial_coordinate));
        map_circle_start_->setProperty("center",QVariant::fromValue(initial_coordinate));
        rotation_state_ = EKF_INS::Utils::toEulerAngles(ekf_->getOrientationState());
        double azimuth = EKF_INS::Utils::constrainAngleDegree(EKF_INS::Utils::radianToDegree(rotation_state_(2)));
        QString azimuth_str = QString::number(azimuth) + degree_str_;
        azimuth_text_->setProperty("text", azimuth_str);
    }
}

void MapUpdater::slotStartButtonClicked() {
    if (!is_start_button_clicked_ && is_load_button_clicked_) {
        is_start_button_clicked_ = true;
        playing_thread_ = new std::thread(&kitti_dataset_tools::KittiDatasetParser::startPlayingData, parser_);
        map_circle_route_->setProperty("visible",true);
    }
}

void MapUpdater::slotUpdateMapRoute() {
    // Update position circle
    position_state_ = ekf_->getPositionState();
    velocity_state_ = ekf_->getVelocityState();
    QGeoCoordinate new_coordinate(EKF_INS::Utils::radianToDegree(position_state_(0)),EKF_INS::Utils::radianToDegree(position_state_(1)));
    osm_map_->setProperty("center",QVariant::fromValue(new_coordinate));
    map_circle_route_->setProperty("center",QVariant::fromValue(new_coordinate));
    double currecnt_vel_km_h = std::sqrt(velocity_state_(0) * velocity_state_(0) + velocity_state_(1) * velocity_state_(1)) * 3.6;
    QString current_vel_str = QString::number(currecnt_vel_km_h) + km_h_str_;
    velocity_text_->setProperty("text", current_vel_str);
    rotation_state_ = EKF_INS::Utils::toEulerAngles(ekf_->getOrientationState());
    double azimuth = EKF_INS::Utils::constrainAngleDegree(EKF_INS::Utils::radianToDegree(rotation_state_(2)));
    QString azimuth_str = QString::number(azimuth) + degree_str_;
    azimuth_text_->setProperty("text", azimuth_str);
}


