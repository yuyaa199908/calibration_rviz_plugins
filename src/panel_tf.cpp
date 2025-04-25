#include "panel_tf.hpp"
#include <cmath> 

#include <rviz_common/config.hpp>
#include <rviz_common/display_context.hpp>

// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QSignalMapper>

namespace calibration_rviz_plugins
{

LabeledSliderWidget::LabeledSliderWidget(QWidget* parent,
                                            const QString& name,
                                            double init_value,
                                            double init_min,
                                            double init_max,
                                            int scale): QWidget( parent )
                                            {
    setMinimumSize(100,100);
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    scale_ = scale;
    value_ = init_value;
    min_ = init_min;
    max_ = init_max;
    if(min_ > value_)value_ = min_;
    if(max_ < value_)max_ = value_;

    label_name_ = new QLabel(name);

    spin_min_ = new QDoubleSpinBox;
    spin_min_->setRange(-1000.0, 1000.0);
    spin_min_->setValue(min_);
    spin_min_->setDecimals(static_cast<int>(std::log10(scale_)));
    spin_min_->setSingleStep(static_cast<double>(1.0/scale_));

    spin_max_ = new QDoubleSpinBox;
    spin_max_->setRange(-1000.0, 1000.0);
    spin_max_->setValue(max_);
    spin_max_->setDecimals(static_cast<int>(std::log10(scale_)));
    spin_max_->setSingleStep(static_cast<double>(1.0/scale_));

    // label_value_ = new QLabel(QString::number(value_ , 'f', static_cast<int>(std::log10(scale_))));
    spin_value_ = new QDoubleSpinBox;
    spin_value_->setRange(min_, max_);
    spin_value_->setValue(value_);
    spin_value_->setDecimals(static_cast<int>(std::log10(scale_)));
    spin_value_->setSingleStep(static_cast<double>(1.0/scale_));

    slider_ = new QSlider(Qt::Horizontal);
    slider_->setRange(int(min_ * scale_), int(max_ * scale_));
    slider_->setValue(int(value_  * scale_));

    // スライダー変更時
    connect(slider_, &QSlider::valueChanged, this, &LabeledSliderWidget::update_slider_value);
    connect(spin_value_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &LabeledSliderWidget::update_spin_value);
    connect(spin_min_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &LabeledSliderWidget::update_min);
    connect(spin_max_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &LabeledSliderWidget::update_max);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(label_name_);
    layout->addWidget(spin_value_);
    layout->addWidget(spin_min_);
    layout->addWidget(slider_);
    layout->addWidget(spin_max_);
    setLayout(layout);
}

void LabeledSliderWidget::update_slider_value(int x)
{
    value_ = static_cast<double>(x) / scale_;
    spin_value_->setValue(value_);
    // label_value_->setText(QString::number(value_, 'f', static_cast<int>(std::log10(scale_))));
    // Q_EMIT modifyPosition(label_value_->text());
}
void LabeledSliderWidget::update_spin_value(double x)
{
    if(x < min_) x = min_;
    if(x > max_) x = max_;
    value_ = x;
    slider_->setValue(int(value_  * scale_));
}
void LabeledSliderWidget::update_min(double x)
{
    if(x > value_) x = value_;
    min_ = x;
    // spin_min_->setValue(min_);
    slider_->setRange(int(min_ * scale_), int(max_ * scale_));
}
void LabeledSliderWidget::update_max(double x)
{
    if(x < value_) x = value_;
    max_ = x;
    // spin_max_->setValue(max_);
    slider_->setRange(int(min_ * scale_), int(max_ * scale_));
}

PanelTF::PanelTF(QWidget *parent): rviz_common::Panel(parent)
{
    QVBoxLayout* main_layout = new QVBoxLayout;

    // frame name (parent, child
    QHBoxLayout* layout_1 = new QHBoxLayout;
    layout_1->addWidget(new QLabel("Parent:"));
    QLineEdit* edit_parent_ = new QLineEdit("");
    layout_1->addWidget(edit_parent_);
    layout_1->addWidget(new QLabel("Child:"));
    QLineEdit* edit_child_ = new QLineEdit("");
    layout_1->addWidget(edit_child_);
    main_layout->addLayout(layout_1);
    // trans_x
    QHBoxLayout* layout_2 = new QHBoxLayout;
    LabeledSliderWidget* labeled_slider_tx = new LabeledSliderWidget(nullptr,"Tx",0.0,-1.0,1.0,1000);
    layout_2->addWidget(labeled_slider_tx);
    main_layout->addLayout(layout_2);
    // trans_y
    QHBoxLayout* layout_3 = new QHBoxLayout;
    LabeledSliderWidget* labeled_slider_ty = new LabeledSliderWidget(nullptr,"Ty",0.0,-1.0,1.0,1000);
    layout_3->addWidget(labeled_slider_ty);
    main_layout->addLayout(layout_3);
    // trans_z
    QHBoxLayout* layout_4 = new QHBoxLayout;
    LabeledSliderWidget* labeled_slider_tz = new LabeledSliderWidget(nullptr,"Tz",0.0,-1.0,1.0,1000);
    layout_4->addWidget(labeled_slider_tz);
    main_layout->addLayout(layout_4);
    // rot_x
    QHBoxLayout* layout_5 = new QHBoxLayout;
    LabeledSliderWidget* labeled_slider_rx = new LabeledSliderWidget(nullptr,"Rx",0.0,0.0,360.0,1000);
    layout_5->addWidget(labeled_slider_rx);
    main_layout->addLayout(layout_5);
    // rot_y
    QHBoxLayout* layout_6 = new QHBoxLayout;
    LabeledSliderWidget* labeled_slider_ry = new LabeledSliderWidget(nullptr,"Ry",0.0,0.0,360.0,1000);
    layout_6->addWidget(labeled_slider_ry);
    main_layout->addLayout(layout_6);
    // rot_z
    QHBoxLayout* layout_7 = new QHBoxLayout;
    LabeledSliderWidget* labeled_slider_rz = new LabeledSliderWidget(nullptr,"Rz",0.0,0.0,360.0,1000);
    layout_7->addWidget(labeled_slider_rz);
    main_layout->addLayout(layout_7);


    setLayout(main_layout);
    QTimer* output_timer = new QTimer(this);
    connect(output_timer, SIGNAL(timeout()), this, SLOT(tick()));
    output_timer->start(100);
}

void PanelTF::onInitialize()
{
    nh_ = this->getDisplayContext()->getRosNodeAbstraction().lock()->get_raw_node();
    string_publisher_ = nh_->create_publisher<std_msgs::msg::String>("test_string", rclcpp::QoS(1));
}

void PanelTF::tick()
{
    // auto msg = std::make_shared<std_msgs::msg::String>();
    // msg->data = "Hello, world!";
    // string_publisher_->publish(*msg);

}

void PanelTF::save(rviz_common::Config config) const
{
    rviz_common::Panel::save(config);
}

void PanelTF::load(const rviz_common::Config &config)
{
    rviz_common::Panel::load(config);
}

}

#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(calibration_rviz_plugins::PanelTF, rviz_common::Panel)