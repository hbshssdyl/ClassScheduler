import QtQuick
import QtQuick.Controls
import QtCharts

Rectangle {
    id: root

    property var operateMode
    property var rootController

    color: "#FFFFFF"
    radius: 5

    border {
        color: "#D6D6D6"
        width: 1
    }

    ChartView {
        title: "物理学生人数变化趋势图"
        anchors.fill: parent
        legend.visible: false
        antialiasing: true

        LineSeries {
            // Y轴
            axisY: ValueAxis {
                min: 0
                max: 10
                titleText: "学生人数"
            }
            // X轴：一月到十二月
            axisX: CategoryAxis {
                min: 1
                max: 12
                titleText: "月份"
                labelsPosition: CategoryAxis.AxisLabelsPositionOnValue
                CategoryRange { label: "一月"; endValue: 1 }
                CategoryRange { label: "二月"; endValue: 2 }
                CategoryRange { label: "三月"; endValue: 3 }
                CategoryRange { label: "四月"; endValue: 4 }
                CategoryRange { label: "五月"; endValue: 5 }
                CategoryRange { label: "六月"; endValue: 6 }
                CategoryRange { label: "七月"; endValue: 7 }
                CategoryRange { label: "八月"; endValue: 8 }
                CategoryRange { label: "九月"; endValue: 9 }
                CategoryRange { label: "十月"; endValue: 10 }
                CategoryRange { label: "十一月"; endValue: 11 }
                CategoryRange { label: "十二月"; endValue: 12 }
            }

            XYPoint { x: 1; y: 4 }
            XYPoint { x: 2; y: 4 }
            XYPoint { x: 3; y: 4 }
            XYPoint { x: 4; y: 3 }
            XYPoint { x: 5; y: 5 }
            // 其余月份可补充数据
        }
    }
}
