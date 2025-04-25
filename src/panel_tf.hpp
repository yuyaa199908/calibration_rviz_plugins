#ifndef CALIBRATION_RVIZ_PLUGINS_PANEL_TF_HPP
#define CALIBRATION_RVIZ_PLUGINS_PANEL_TF_HPP

#ifndef Q_MOC_RUN
#include <rclcpp/rclcpp.hpp>
#include <rviz_common/panel.hpp>
#include <QtWidgets>
#include <QSlider>
#endif
#include <std_msgs/msg/string.hpp>

namespace calibration_rviz_plugins
{
class LabeledSliderWidget : public QWidget
{
    Q_OBJECT
public:
    LabeledSliderWidget(QWidget *parent = 0,
                        const QString& name = "hoge",
                        double init_value = 0.0,
                        double init_min = -1.0,
                        double init_max = 1.0,
                        int scale = 1000);

    QLabel* label_name_;
    // QLabel* label_value_;
    QDoubleSpinBox* spin_value_;
    QSlider* slider_;
    QDoubleSpinBox* spin_min_;
    QDoubleSpinBox* spin_max_;

    int scale_ ;
    double value_;
    double min_;
    double max_;

    virtual void update_slider_value(int x);
    virtual void update_spin_value(double x);
    virtual void update_min(double x);
    virtual void update_max(double x);
};

class PanelTF : public rviz_common::Panel
{
    Q_OBJECT
public:
    PanelTF(QWidget *parent = nullptr);

    virtual void onInitialize();
    virtual void load(const rviz_common::Config &config);
    virtual void save(rviz_common::Config config) const;

public Q_SLOTS:
    void tick();


protected:
    const int scale = 1000;
    rclcpp::Node::SharedPtr nh_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr string_publisher_;
};

}

#endif // SAMPLE_RVIZ_PLUGINS_PANEL_TF_HPP