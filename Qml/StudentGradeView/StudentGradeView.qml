import Qt5Compat.GraphicalEffects
import QtCharts 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    id: root

    // Search functionality
    property string searchText: ""
    property var filteredStudentModel
    // Student data with integrated scores - 学生数据结构（包含成绩信息）
    property var studentsData: [{
        "id": "student_001",
        "name": "王五",
        "grade": "高二",
        "school": "第二中学",
        "phone": "13700137000",
        "teachers": "数学：李老师，物理：陈老师",
        "scoresData": []
    }, {
        "id": "student_002",
        "name": "陈六",
        "grade": "高一",
        "school": "第三中学",
        "phone": "13600136000",
        "teachers": "物理：陈老师，英语：赵老师",
        "scoresData": [{
            "subject": "物理",
            "value": 78,
            "date": "2025-03-01",
            "examType": "周考"
        }, {
            "subject": "物理",
            "value": 85,
            "date": "2025-04-01",
            "examType": "月考"
        }, {
            "subject": "英语",
            "value": 76,
            "date": "2025-03-20",
            "examType": "月考"
        }]
    }, {
        "id": "student_003",
        "name": "李四",
        "grade": "初二",
        "school": "实验中学",
        "phone": "13900139000",
        "teachers": "英语：赵老师，语文：王老师",
        "scoresData": [{
            "subject": "英语",
            "value": 70,
            "date": "2025-03-01",
            "examType": "周考"
        }, {
            "subject": "英语",
            "value": 78,
            "date": "2025-04-01",
            "examType": "月考"
        }, {
            "subject": "英语",
            "value": 85,
            "date": "2025-05-01",
            "examType": "期中考试"
        }, {
            "subject": "语文",
            "value": 88,
            "date": "2025-03-10",
            "examType": "月考"
        }]
    }, {
        "id": "student_004",
        "name": "张三",
        "grade": "高一",
        "school": "第一中学",
        "phone": "13800138000",
        "teachers": "数学：王老师，英语：李老师",
        "scoresData": [{
            "subject": "数学",
            "value": 72,
            "date": "2025-02-01",
            "examType": "周考"
        }, {
            "subject": "数学",
            "value": 78,
            "date": "2025-03-01",
            "examType": "月考"
        }, {
            "subject": "数学",
            "value": 85,
            "date": "2025-04-01",
            "examType": "期中考试"
        }, {
            "subject": "数学",
            "value": 91,
            "date": "2025-05-01",
            "examType": "期末考试"
        }, {
            "subject": "英语",
            "value": 68,
            "date": "2025-02-15",
            "examType": "周考"
        }, {
            "subject": "英语",
            "value": 75,
            "date": "2025-03-15",
            "examType": "月考"
        }]
    }]

    function performSearch() {
        filteredStudentModel.clear();
        if (searchText.trim() === "") {
            // If search is empty, show all students
            for (let i = 0; i < studentModel.count; i++) {
                let student = studentModel.get(i);
                filteredStudentModel.append(student);
            }
        } else {
            // Filter students based on search text
            let searchLower = searchText.toLowerCase();
            for (let i = 0; i < studentModel.count; i++) {
                let student = studentModel.get(i);
                let matchFound = false;
                // Check name
                if (student.name.toLowerCase().includes(searchLower))
                    matchFound = true;

                // Check school
                if (student.school.toLowerCase().includes(searchLower))
                    matchFound = true;

                // Check grade
                if (student.grade.toLowerCase().includes(searchLower))
                    matchFound = true;

                // Check phone
                if (student.phone.includes(searchText))
                    matchFound = true;

                // Check teachers
                if (student.teachers.toLowerCase().includes(searchLower))
                    matchFound = true;

                if (matchFound)
                    filteredStudentModel.append(student);

            }
        }
    }

    // JavaScript function to get subjects from scores
    function getSubjects(scores) {
        if (!scores || scores.length === 0)
            return [];

        var subjectSet = {
        };
        for (var i = 0; i < scores.length; i++) {
            subjectSet[scores[i].subject] = true;
        }
        return Object.keys(subjectSet);
    }

    // Helper function to convert ListModel to JavaScript array
    function listModelToArray(listModel) {
        let result = [];
        if (listModel && listModel.count !== undefined) {
            for (let i = 0; i < listModel.count; i++) {
                result.push(listModel.get(i));
            }
        } else if (listModel && Array.isArray(listModel)) {
            result = listModel;
        }
        return result;
    }

    // Function to get student scores by student ID
    function getStudentScores(studentId) {
        for (let i = 0; i < studentsData.length; i++) {
            if (studentsData[i].id === studentId)
                return listModelToArray(studentsData[i].scoresData);

        }
        return [];
    }

    Component.onCompleted: {
        // Initialize student model from data
        for (let i = 0; i < studentsData.length; i++) {
            studentModel.append(studentsData[i]);
        }
        // Initialize filtered model with all students
        performSearch();
    }

    // Student list model
    ListModel {
        id: studentModel
    }

    // Student list view
    ListView {
        id: studentList

        anchors.fill: parent
        anchors.margins: 15
        spacing: 20
        clip: true
        model: filteredStudentModel

        delegate: Rectangle {
            width: parent ? parent.width - 10 : 1150
            height: studentContent.implicitHeight + 28 // 根据内容自动调整高度
            color: "white"
            radius: 12
            border.color: "#E9ECEF"
            border.width: 1
            anchors.horizontalCenter: parent ? parent.horizontalCenter : undefined
            layer.enabled: true

            Column {
                id: studentContent

                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: 16
                spacing: 12

                // Student Information Card (统一美观设计)
                Rectangle {
                    id: studentInfoCard

                    property bool showTeacherInfo: false

                    width: parent.width
                    height: studentInfoContent.implicitHeight + 24
                    radius: 12
                    color: "#FFFFFF"
                    border.color: "#E0E7FF"
                    border.width: 2
                    // 阴影效果
                    layer.enabled: true

                    Column {
                        id: studentInfoContent

                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.margins: 12
                        spacing: 16

                        // Header with avatar and name
                        Item {
                            width: parent.width
                            height: 70

                            // Avatar circle
                            Rectangle {
                                id: avatarCircle

                                width: 60
                                height: 60
                                radius: 30
                                color: "#6366F1"
                                border.color: "#4F46E5"
                                border.width: 3
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter

                                Text {
                                    anchors.centerIn: parent
                                    text: name.charAt(0)
                                    font.bold: true
                                    font.pixelSize: 24
                                    color: "white"
                                }

                            }

                            // Name and basic info
                            Column {
                                anchors.left: avatarCircle.right
                                anchors.leftMargin: 16
                                anchors.right: teacherToggleButton.left
                                anchors.rightMargin: 12
                                anchors.verticalCenter: parent.verticalCenter
                                spacing: 4

                                TextEdit {
                                    text: name
                                    font.bold: true
                                    font.pixelSize: 22
                                    color: "#1E293B"
                                    selectByMouse: true
                                    readOnly: true
                                    wrapMode: TextEdit.NoWrap
                                }

                                Flow {
                                    width: parent.width
                                    spacing: 20

                                    Row {
                                        spacing: 6

                                        Text {
                                            text: "🎓"
                                            font.pixelSize: 14
                                            color: "#6366F1"
                                        }

                                        TextEdit {
                                            text: grade
                                            font.pixelSize: 14
                                            color: "#64748B"
                                            font.weight: Font.Medium
                                            selectByMouse: true
                                            readOnly: true
                                            wrapMode: TextEdit.NoWrap
                                        }

                                    }

                                    Row {
                                        spacing: 6

                                        Text {
                                            text: "🏫"
                                            font.pixelSize: 14
                                            color: "#6366F1"
                                        }

                                        TextEdit {
                                            text: school
                                            font.pixelSize: 14
                                            color: "#64748B"
                                            font.weight: Font.Medium
                                            selectByMouse: true
                                            readOnly: true
                                            wrapMode: TextEdit.NoWrap
                                        }

                                    }

                                    Row {
                                        spacing: 6

                                        Text {
                                            text: "📱"
                                            font.pixelSize: 14
                                            color: "#6366F1"
                                        }

                                        TextEdit {
                                            text: phone
                                            font.pixelSize: 14
                                            color: "#64748B"
                                            font.weight: Font.Medium
                                            selectByMouse: true
                                            readOnly: true
                                            wrapMode: TextEdit.NoWrap
                                        }

                                    }

                                }

                            }

                            // 教师信息展示/隐藏按钮 (最右侧)
                            Column {
                                id: teacherToggleButton

                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                spacing: 4

                                Rectangle {
                                    width: 45
                                    height: 35
                                    radius: 8
                                    color: studentInfoCard.showTeacherInfo ? "#22C55E" : "#64748B"
                                    border.color: studentInfoCard.showTeacherInfo ? "#16A34A" : "#475569"
                                    border.width: 2
                                    anchors.horizontalCenter: parent.horizontalCenter

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            studentInfoCard.showTeacherInfo = !studentInfoCard.showTeacherInfo;
                                        }
                                    }

                                    Text {
                                        anchors.centerIn: parent
                                        text: studentInfoCard.showTeacherInfo ? "👨‍🏫" : "👤"
                                        font.pixelSize: 16
                                        color: "white"
                                    }

                                }

                                Text {
                                    text: studentInfoCard.showTeacherInfo ? "隐藏教师" : "显示教师"
                                    font.pixelSize: 10
                                    color: "#64748B"
                                    font.weight: Font.Medium
                                    anchors.horizontalCenter: parent.horizontalCenter
                                }

                            }

                        }

                        // Divider line (更明显的分割线)
                        Rectangle {
                            width: parent.width
                            height: 3
                            radius: 1.5
                            color: "#CBD5E1"
                            visible: studentInfoCard.showTeacherInfo

                            // 渐变效果让分割线更明显
                            gradient: Gradient {
                                GradientStop {
                                    position: 0
                                    color: "#E2E8F0"
                                }

                                GradientStop {
                                    position: 0.5
                                    color: "#CBD5E1"
                                }

                                GradientStop {
                                    position: 1
                                    color: "#E2E8F0"
                                }

                            }

                        }

                        // Teacher information (可隐藏)
                        Row {
                            width: parent.width
                            spacing: 12
                            visible: studentInfoCard.showTeacherInfo

                            // Teacher icon background
                            Rectangle {
                                width: 40
                                height: 40
                                radius: 20
                                color: "#F0FDF4"
                                border.color: "#22C55E"
                                border.width: 2

                                Text {
                                    anchors.centerIn: parent
                                    text: "👨‍🏫"
                                    font.pixelSize: 18
                                }

                            }

                            Column {
                                anchors.verticalCenter: parent.verticalCenter
                                spacing: 4

                                Text {
                                    text: "授课教师"
                                    font.pixelSize: 12
                                    color: "#64748B"
                                    font.weight: Font.Medium
                                }

                                TextEdit {
                                    text: teachers
                                    font.pixelSize: 15
                                    color: "#1E293B"
                                    font.weight: Font.Medium
                                    width: studentInfoCard.width - 80
                                    wrapMode: TextEdit.Wrap
                                    selectByMouse: true
                                    readOnly: true
                                }

                            }

                        }

                    }

                    // 渐变背景
                    gradient: Gradient {
                        GradientStop {
                            position: 0
                            color: "#FAFBFF"
                        }

                        GradientStop {
                            position: 1
                            color: "#F0F4FF"
                        }

                    }

                    layer.effect: DropShadow {
                        horizontalOffset: 0
                        verticalOffset: 2
                        radius: 8
                        samples: 16
                        color: "#1E293B20"
                    }

                }

                // Charts container with subject visibility controls
                Rectangle {
                    id: chartsContainer

                    property var subjectVisibility: ({
                    })
                    property var availableSubjects: []
                    property bool showAllScoreDetails: false
                    property bool showCombinedChart: true
                    property bool hasScoreData: {
                        let scores = listModelToArray(scoresData);
                        return scores && scores.length > 0;
                    }

                    signal visibilityUpdated()

                    function initializeSubjects() {
                        let scores = listModelToArray(scoresData);
                        if (!scores || scores.length === 0)
                            return ;

                        // Get unique subjects
                        let subjects = new Set();
                        for (let i = 0; i < scores.length; i++) {
                            subjects.add(scores[i].subject);
                        }
                        availableSubjects = Array.from(subjects);
                        // Initialize all subjects as hidden (not visible)
                        for (let subject of availableSubjects) {
                            subjectVisibility[subject] = false;
                        }
                        // Trigger update
                        visibilityUpdated();
                    }

                    function toggleSubjectVisibility(subject) {
                        subjectVisibility[subject] = !subjectVisibility[subject];
                        visibilityUpdated();
                    }

                    width: parent.width
                    height: hasScoreData ? (chartsColumn.height + 32) : 0
                    radius: 12
                    color: "#FFFFFF"
                    border.color: "#E0E7FF"
                    border.width: 2
                    visible: hasScoreData
                    Component.onCompleted: {
                        initializeSubjects();
                    }

                    Column {
                        id: chartsColumn

                        width: parent.width - 32
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.margins: 16
                        spacing: 12

                        // Combined chart showing all subjects
                        Rectangle {
                            width: parent.width
                            height: 320
                            radius: 12
                            color: "#FFFFFF"
                            border.color: "#E1E5E9"
                            border.width: 2
                            visible: chartsContainer.showCombinedChart

                            Column {
                                anchors.fill: parent
                                anchors.margins: 8
                                spacing: 4

                                // Combined chart title
                                Rectangle {
                                    width: parent.width
                                    height: 28
                                    radius: 6
                                    color: "#E8F5E9"
                                    border.color: "#4CAF50"
                                    border.width: 2

                                    Text {
                                        anchors.centerIn: parent
                                        text: "📈 " + name + " 的综合成绩趋势（全科目对比）"
                                        font.bold: true
                                        font.pixelSize: 14
                                        color: "#2E7D32"
                                    }

                                }

                                // Combined Chart
                                ChartView {
                                    property var allScoresData: []

                                    function createCombinedChart() {
                                        let scores = listModelToArray(scoresData);
                                        if (!scores || scores.length === 0)
                                            return ;

                                        allScoresData = scores;
                                        // Find the maximum number of tests for any subject to set axis range
                                        let maxTests = 0;
                                        for (let subject of chartsContainer.availableSubjects) {
                                            let subjectScores = scores.filter((s) => {
                                                return s.subject === subject;
                                            });
                                            if (subjectScores.length > maxTests)
                                                maxTests = subjectScores.length;

                                        }
                                        // Set axis range based on maximum tests
                                        axisXCombined.max = Math.max(maxTests, 2);
                                        axisXCombined.tickCount = Math.max(maxTests, 2);
                                        // Create series for each subject
                                        for (let subject of chartsContainer.availableSubjects) {
                                            let subjectScores = scores.filter((s) => {
                                                return s.subject === subject;
                                            });
                                            if (subjectScores.length === 0)
                                                continue;

                                            // Sort subject scores by date
                                            subjectScores.sort((a, b) => {
                                                return new Date(a.date) - new Date(b.date);
                                            });
                                            let series = createSeries(ChartView.SeriesTypeLine, subject, axisXCombined, axisYCombined);
                                            series.color = getSubjectColor(subject);
                                            series.width = 3;
                                            series.pointsVisible = true;
                                            // Add data points with sequential index (1, 2, 3, ...)
                                            for (let i = 0; i < subjectScores.length; i++) {
                                                series.append(i + 1, subjectScores[i].value);
                                            }
                                        }
                                    }

                                    function getSubjectColor(subject) {
                                        let colors = {
                                            "数学": "#FF6B6B",
                                            "英语": "#4ECDC4",
                                            "物理": "#45B7D1",
                                            "化学": "#96CEB4",
                                            "语文": "#FFEAA7",
                                            "生物": "#DDA0DD",
                                            "地理": "#FFB347",
                                            "历史": "#87CEEB",
                                            "政治": "#98FB98"
                                        };
                                        return colors[subject] || "#FF6B6B";
                                    }

                                    width: parent.width
                                    height: parent.height - 32
                                    antialiasing: true
                                    theme: ChartView.ChartThemeLight
                                    legend.visible: true
                                    legend.alignment: Qt.AlignBottom
                                    backgroundColor: "transparent"
                                    Component.onCompleted: {
                                        createCombinedChart();
                                    }

                                    // Mouse area for hover functionality
                                    MouseArea {
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        onPositionChanged: {
                                            let point = parent.mapToValue(Qt.point(mouseX, mouseY));
                                            let closestXIndex = Math.round(point.x);
                                            // Find closest data point across all subjects
                                            let closestScore = null;
                                            let minDistance = 1000;
                                            let scores = listModelToArray(scoresData);
                                            // Check each subject
                                            for (let subject of chartsContainer.availableSubjects) {
                                                let subjectScores = scores.filter((s) => {
                                                    return s.subject === subject;
                                                });
                                                if (subjectScores.length === 0)
                                                    continue;

                                                // Sort subject scores by date
                                                subjectScores.sort((a, b) => {
                                                    return new Date(a.date) - new Date(b.date);
                                                });
                                                // Check if the x index is within range for this subject
                                                if (closestXIndex >= 1 && closestXIndex <= subjectScores.length) {
                                                    let scoreData = subjectScores[closestXIndex - 1];
                                                    let dataPoint = parent.mapToPosition(Qt.point(closestXIndex, scoreData.value));
                                                    let distance = Math.sqrt(Math.pow(mouseX - dataPoint.x, 2) + Math.pow(mouseY - dataPoint.y, 2));
                                                    if (distance < minDistance && distance <= 25) {
                                                        minDistance = distance;
                                                        closestScore = scoreData;
                                                    }
                                                }
                                            }
                                            if (closestScore) {
                                                hoverLabelCombined.visible = true;
                                                hoverLabelCombined.x = Math.min(mouseX + 10, parent.width - hoverLabelCombined.width);
                                                hoverLabelCombined.y = Math.max(mouseY - 40, 0);
                                                hoverLabelCombined.text = closestScore.subject + "\n成绩: " + closestScore.value + "\n日期: " + closestScore.date + "\n考试: " + closestScore.examType;
                                            } else {
                                                hoverLabelCombined.visible = false;
                                            }
                                        }
                                        onExited: {
                                            hoverLabelCombined.visible = false;
                                        }
                                    }

                                    // Hover tooltip for combined chart
                                    Rectangle {
                                        id: hoverLabelCombined

                                        property alias text: hoverTextCombined.text

                                        visible: false
                                        width: hoverTextCombined.implicitWidth + 16
                                        height: hoverTextCombined.implicitHeight + 12
                                        color: "#333333"
                                        radius: 6
                                        border.color: "#555555"
                                        border.width: 1
                                        z: 1000
                                        layer.enabled: true

                                        Text {
                                            id: hoverTextCombined

                                            anchors.centerIn: parent
                                            color: "white"
                                            font.pixelSize: 11
                                            font.weight: Font.Medium
                                        }

                                        layer.effect: DropShadow {
                                            horizontalOffset: 2
                                            verticalOffset: 2
                                            radius: 6
                                            samples: 13
                                            color: "#80000000"
                                        }

                                    }

                                    // Score labels overlay for combined chart
                                    Repeater {
                                        model: chartsContainer.showAllScoreDetails ? parent.allScoresData : []

                                        delegate: Rectangle {
                                            property var scores: listModelToArray(scoresData)
                                            property int dateIndex: {
                                                // Find the index of this score within its subject's sorted scores
                                                let subjectScores = scores.filter((s) => {
                                                    return s.subject === modelData.subject;
                                                });
                                                subjectScores.sort((a, b) => {
                                                    return new Date(a.date) - new Date(b.date);
                                                });
                                                for (let i = 0; i < subjectScores.length; i++) {
                                                    if (subjectScores[i].date === modelData.date && subjectScores[i].value === modelData.value)
                                                        return i + 1;

                                                }
                                                return 1;
                                            }
                                            property point dataPos: parent.mapToPosition(Qt.point(dateIndex, modelData.value))

                                            x: dataPos.x - width / 2
                                            y: dataPos.y - height - 8
                                            width: combinedScoreLabel.implicitWidth + 12
                                            height: combinedScoreLabel.implicitHeight + 8
                                            color: parent.getSubjectColor(modelData.subject)
                                            radius: 4
                                            border.color: "#FFFFFF"
                                            border.width: 2
                                            z: 999
                                            layer.enabled: true

                                            Text {
                                                id: combinedScoreLabel

                                                anchors.centerIn: parent
                                                text: modelData.subject + "\n" + modelData.value + "分\n" + modelData.date
                                                color: "white"
                                                font.pixelSize: 9
                                                font.weight: Font.Bold
                                                horizontalAlignment: Text.AlignHCenter
                                            }

                                            layer.effect: DropShadow {
                                                horizontalOffset: 1
                                                verticalOffset: 1
                                                radius: 4
                                                samples: 9
                                                color: "#80000000"
                                            }

                                        }

                                    }

                                    ValueAxis {
                                        id: axisXCombined

                                        min: 1
                                        titleText: "考试序号"
                                        minorTickCount: 0
                                        labelFormat: "%.0f"
                                    }

                                    ValueAxis {
                                        id: axisYCombined

                                        min: 0
                                        max: 150
                                        titleText: "成绩"
                                    }

                                }

                            }

                        }

                        // Subject charts container (without internal scroll)
                        Column {
                            width: parent.width
                            spacing: 16

                            Repeater {
                                id: chartsRepeater

                                model: chartsContainer.availableSubjects

                                delegate: Rectangle {
                                    property bool isVisible: chartsContainer.subjectVisibility[modelData] || false

                                    width: parent.width
                                    height: 280
                                    radius: 12
                                    color: "#FFFFFF"
                                    border.color: "#E1E5E9"
                                    border.width: 2
                                    visible: isVisible

                                    Connections {
                                        function onVisibilityUpdated() {
                                            isVisible = chartsContainer.subjectVisibility[modelData] || false;
                                        }

                                        target: chartsContainer
                                    }

                                    Column {
                                        anchors.fill: parent
                                        anchors.margins: 8
                                        spacing: 4

                                        // Subject title
                                        Rectangle {
                                            width: parent.width
                                            height: 26
                                            radius: 6
                                            color: "#E8F5E9"
                                            border.color: "#4CAF50"
                                            border.width: 1

                                            Text {
                                                anchors.centerIn: parent
                                                text: modelData + " 成绩趋势"
                                                font.bold: true
                                                font.pixelSize: 14
                                                color: "#2E7D32"
                                            }

                                        }

                                        // Chart
                                        ChartView {
                                            property var currentSubjectScores: []

                                            function createSubjectChart(subject) {
                                                let scores = listModelToArray(scoresData);
                                                if (!scores || scores.length === 0)
                                                    return ;

                                                // Filter scores for this subject
                                                let subjectScores = scores.filter((score) => {
                                                    return score.subject === subject;
                                                });
                                                if (subjectScores.length === 0)
                                                    return ;

                                                // Store scores for hover functionality
                                                currentSubjectScores = subjectScores;
                                                // Set axis range
                                                let maxPoints = subjectScores.length;
                                                if (maxPoints === 1) {
                                                    axisX.max = 2;
                                                    axisX.tickCount = 2;
                                                } else {
                                                    axisX.max = maxPoints;
                                                    axisX.tickCount = maxPoints;
                                                }
                                                // Create series
                                                let series = createSeries(ChartView.SeriesTypeLine, subject, axisX, axisY);
                                                series.color = getSubjectColor(subject);
                                                series.width = 3;
                                                series.pointsVisible = true;
                                                // Add data points
                                                for (let i = 0; i < subjectScores.length; i++) {
                                                    series.append(i + 1, subjectScores[i].value);
                                                }
                                            }

                                            function getSubjectColor(subject) {
                                                let colors = {
                                                    "数学": "#FF6B6B",
                                                    "英语": "#4ECDC4",
                                                    "物理": "#45B7D1",
                                                    "化学": "#96CEB4",
                                                    "语文": "#FFEAA7",
                                                    "生物": "#DDA0DD",
                                                    "地理": "#FFB347",
                                                    "历史": "#87CEEB",
                                                    "政治": "#98FB98"
                                                };
                                                return colors[subject] || "#FF6B6B";
                                            }

                                            width: parent.width
                                            height: parent.height - 30
                                            antialiasing: true
                                            theme: ChartView.ChartThemeLight
                                            legend.visible: false
                                            backgroundColor: "transparent"
                                            Component.onCompleted: {
                                                createSubjectChart(modelData);
                                            }

                                            // Mouse area for hover functionality
                                            MouseArea {
                                                anchors.fill: parent
                                                hoverEnabled: true
                                                onPositionChanged: {
                                                    let point = parent.mapToValue(Qt.point(mouseX, mouseY));
                                                    let closestIndex = Math.round(point.x) - 1;
                                                    // Check if mouse is close enough to a data point
                                                    let foundPoint = false;
                                                    if (closestIndex >= 0 && closestIndex < parent.currentSubjectScores.length) {
                                                        // Convert the data point back to screen coordinates
                                                        let dataPoint = parent.mapToPosition(Qt.point(closestIndex + 1, parent.currentSubjectScores[closestIndex].value));
                                                        let distance = Math.sqrt(Math.pow(mouseX - dataPoint.x, 2) + Math.pow(mouseY - dataPoint.y, 2));
                                                        // Only show tooltip if mouse is within 20 pixels of the data point
                                                        if (distance <= 20) {
                                                            let scoreData = parent.currentSubjectScores[closestIndex];
                                                            hoverLabel.visible = true;
                                                            hoverLabel.x = Math.min(mouseX + 10, parent.width - hoverLabel.width);
                                                            hoverLabel.y = Math.max(mouseY - 40, 0);
                                                            hoverLabel.text = "成绩: " + scoreData.value + "\n日期: " + scoreData.date + "\n考试: " + scoreData.examType;
                                                            foundPoint = true;
                                                        }
                                                    }
                                                    if (!foundPoint)
                                                        hoverLabel.visible = false;

                                                }
                                                onExited: {
                                                    hoverLabel.visible = false;
                                                }
                                            }

                                            // Hover tooltip
                                            Rectangle {
                                                id: hoverLabel

                                                property alias text: hoverText.text

                                                visible: false
                                                width: hoverText.implicitWidth + 16
                                                height: hoverText.implicitHeight + 12
                                                color: "#333333"
                                                radius: 6
                                                border.color: "#555555"
                                                border.width: 1
                                                z: 1000
                                                layer.enabled: true

                                                Text {
                                                    id: hoverText

                                                    anchors.centerIn: parent
                                                    color: "white"
                                                    font.pixelSize: 11
                                                    font.weight: Font.Medium
                                                }

                                                layer.effect: DropShadow {
                                                    horizontalOffset: 2
                                                    verticalOffset: 2
                                                    radius: 6
                                                    samples: 13
                                                    color: "#80000000"
                                                }

                                            }

                                            // Score labels overlay for all data points
                                            Repeater {
                                                model: chartsContainer.showAllScoreDetails ? parent.currentSubjectScores : []

                                                delegate: Rectangle {
                                                    property point dataPos: parent.mapToPosition(Qt.point(index + 1, modelData.value))

                                                    x: dataPos.x - width / 2
                                                    y: dataPos.y - height - 8
                                                    width: scoreLabel.implicitWidth + 12
                                                    height: scoreLabel.implicitHeight + 8
                                                    color: "#333333"
                                                    radius: 4
                                                    border.color: "#555555"
                                                    border.width: 1
                                                    z: 999
                                                    layer.enabled: true

                                                    Text {
                                                        id: scoreLabel

                                                        anchors.centerIn: parent
                                                        text: modelData.value + "分\n" + modelData.date + "\n" + modelData.examType
                                                        color: "white"
                                                        font.pixelSize: 10
                                                        font.weight: Font.Medium
                                                        horizontalAlignment: Text.AlignHCenter
                                                    }

                                                    layer.effect: DropShadow {
                                                        horizontalOffset: 1
                                                        verticalOffset: 1
                                                        radius: 4
                                                        samples: 9
                                                        color: "#80000000"
                                                    }

                                                }

                                            }

                                            ValueAxis {
                                                id: axisX

                                                min: 1
                                                titleText: "考试序号"
                                                minorTickCount: 0
                                                labelFormat: "%.0f"
                                            }

                                            ValueAxis {
                                                id: axisY

                                                min: 0
                                                max: 150
                                                titleText: "成绩"
                                            }

                                        }

                                    }

                                }

                            }

                        }

                        // Control switches for each subject
                        Rectangle {
                            width: parent.width
                            height: 85
                            radius: 8
                            color: "#F8F9FA"
                            border.color: "#E1E5E9"
                            border.width: 2

                            Column {
                                anchors.fill: parent
                                anchors.margins: 8
                                spacing: 8

                                Text {
                                    text: "📊 显示控制"
                                    font.bold: true
                                    font.pixelSize: 12
                                    color: "#495057"
                                    height: 20
                                }

                                Flow {
                                    width: parent.width
                                    spacing: 8

                                    // Combined chart control switch
                                    Rectangle {
                                        width: combinedRow.implicitWidth + 16
                                        height: 38
                                        radius: 6
                                        color: combinedChartSwitch.checked ? "#E3F2FD" : "#F5F5F5"
                                        border.color: combinedChartSwitch.checked ? "#2196F3" : "#BDBDBD"
                                        border.width: 1
                                        visible: chartsContainer.hasScoreData

                                        Row {
                                            id: combinedRow

                                            anchors.centerIn: parent
                                            spacing: 3

                                            Text {
                                                text: "📈"
                                                font.pixelSize: 13
                                                color: combinedChartSwitch.checked ? "#1976D2" : "#757575"
                                                anchors.verticalCenter: parent.verticalCenter
                                            }

                                            Text {
                                                text: "综合图表"
                                                font.pixelSize: 11
                                                font.weight: Font.Medium
                                                color: combinedChartSwitch.checked ? "#1976D2" : "#757575"
                                                anchors.verticalCenter: parent.verticalCenter
                                            }

                                            Switch {
                                                id: combinedChartSwitch

                                                checked: chartsContainer.showCombinedChart
                                                onCheckedChanged: {
                                                    chartsContainer.showCombinedChart = checked;
                                                }
                                                anchors.verticalCenter: parent.verticalCenter
                                                scale: 0.7
                                            }

                                        }

                                    }

                                    Repeater {
                                        model: chartsContainer.availableSubjects

                                        delegate: Rectangle {
                                            property bool isActive: chartsContainer.subjectVisibility[modelData] || false

                                            width: subjectRow.implicitWidth + 16
                                            height: 38
                                            radius: 6
                                            color: isActive ? "#E8F5E9" : "#F5F5F5"
                                            border.color: isActive ? "#4CAF50" : "#BDBDBD"
                                            border.width: 1

                                            Connections {
                                                function onVisibilityUpdated() {
                                                    isActive = chartsContainer.subjectVisibility[modelData] || false;
                                                }

                                                target: chartsContainer
                                            }

                                            Row {
                                                id: subjectRow

                                                anchors.centerIn: parent
                                                spacing: 3

                                                Text {
                                                    text: "📚"
                                                    font.pixelSize: 12
                                                    color: parent.parent.isActive ? "#2E7D32" : "#757575"
                                                    anchors.verticalCenter: parent.verticalCenter
                                                }

                                                Text {
                                                    id: subjectText

                                                    text: modelData
                                                    font.pixelSize: 11
                                                    font.weight: Font.Medium
                                                    color: parent.parent.isActive ? "#2E7D32" : "#757575"
                                                    anchors.verticalCenter: parent.verticalCenter
                                                }

                                                Switch {
                                                    id: subjectSwitch

                                                    checked: chartsContainer.subjectVisibility[modelData] || false
                                                    onCheckedChanged: {
                                                        chartsContainer.subjectVisibility[modelData] = checked;
                                                        chartsContainer.visibilityUpdated();
                                                    }
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    scale: 0.7
                                                }

                                            }

                                        }

                                    }

                                    // Show Score Details switch
                                    Rectangle {
                                        width: detailsRow.implicitWidth + 16
                                        height: 38
                                        radius: 6
                                        color: scoreDetailsSwitch.checked ? "#FFF3E0" : "#F5F5F5"
                                        border.color: scoreDetailsSwitch.checked ? "#FF9800" : "#BDBDBD"
                                        border.width: 1
                                        visible: chartsContainer.hasScoreData

                                        Row {
                                            id: detailsRow

                                            anchors.centerIn: parent
                                            spacing: 3

                                            Text {
                                                text: "📋"
                                                font.pixelSize: 13
                                                color: scoreDetailsSwitch.checked ? "#F57C00" : "#757575"
                                                anchors.verticalCenter: parent.verticalCenter
                                            }

                                            Text {
                                                text: "成绩详情"
                                                font.pixelSize: 11
                                                font.weight: Font.Medium
                                                color: scoreDetailsSwitch.checked ? "#F57C00" : "#757575"
                                                anchors.verticalCenter: parent.verticalCenter
                                            }

                                            Switch {
                                                id: scoreDetailsSwitch

                                                checked: chartsContainer.showAllScoreDetails
                                                onCheckedChanged: {
                                                    chartsContainer.showAllScoreDetails = checked;
                                                }
                                                anchors.verticalCenter: parent.verticalCenter
                                                scale: 0.7
                                            }

                                        }

                                    }

                                    // Add Score button - 更加融入的设计
                                    Rectangle {
                                        width: addRow.implicitWidth + 16
                                        height: 38
                                        radius: 6
                                        color: "#F3E5F5"
                                        border.color: "#9C27B0"
                                        border.width: 1

                                        MouseArea {
                                            anchors.fill: parent
                                            onClicked: {
                                                addScoreDialog.currentStudentId = id;
                                                addScoreDialog.currentStudentName = name;
                                                addScoreDialog.open();
                                            }
                                            onPressed: parent.color = "#E1BEE7"
                                            onReleased: parent.color = "#F3E5F5"
                                        }

                                        Row {
                                            id: addRow

                                            anchors.centerIn: parent
                                            spacing: 4

                                            Text {
                                                text: "➕"
                                                font.pixelSize: 13
                                                color: "#7B1FA2"
                                                anchors.verticalCenter: parent.verticalCenter
                                            }

                                            Text {
                                                text: "添加成绩"
                                                font.pixelSize: 11
                                                font.weight: Font.Medium
                                                color: "#7B1FA2"
                                                anchors.verticalCenter: parent.verticalCenter
                                            }

                                        }

                                    }

                                }

                            }

                        }

                    }

                    // 渐变背景
                    gradient: Gradient {
                        GradientStop {
                            position: 0
                            color: "#FAFBFF"
                        }

                        GradientStop {
                            position: 1
                            color: "#F0F4FF"
                        }

                    }

                }

                // No score data message
                Rectangle {
                    width: parent.width
                    height: 120
                    radius: 12
                    color: "#FFFFFF"
                    border.color: "#E0E7FF"
                    border.width: 2
                    visible: !chartsContainer.hasScoreData

                    Column {
                        anchors.centerIn: parent
                        spacing: 12

                        Text {
                            text: "📊"
                            font.pixelSize: 32
                            color: "#94A3B8"
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                        Text {
                            text: "暂无成绩记录"
                            font.bold: true
                            font.pixelSize: 16
                            color: "#64748B"
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                        Text {
                            text: "点击\"添加成绩\"按钮为该学生录入成绩"
                            font.pixelSize: 12
                            color: "#94A3B8"
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                    }

                    // 渐变背景
                    gradient: Gradient {
                        GradientStop {
                            position: 0
                            color: "#FAFBFF"
                        }

                        GradientStop {
                            position: 1
                            color: "#F0F4FF"
                        }

                    }

                }

                // Add Score button for students without score data
                Rectangle {
                    width: parent.width
                    height: 60
                    radius: 12
                    color: "#FFFFFF"
                    border.color: "#E0E7FF"
                    border.width: 2
                    visible: !chartsContainer.hasScoreData

                    Rectangle {
                        width: addRowNoData.implicitWidth + 20
                        height: 40
                        radius: 8
                        color: "#F3E5F5"
                        border.color: "#9C27B0"
                        border.width: 1
                        anchors.centerIn: parent

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                addScoreDialog.currentStudentId = id;
                                addScoreDialog.currentStudentName = name;
                                addScoreDialog.open();
                            }
                            onPressed: parent.color = "#E1BEE7"
                            onReleased: parent.color = "#F3E5F5"
                        }

                        Row {
                            id: addRowNoData

                            anchors.centerIn: parent
                            spacing: 6

                            Text {
                                text: "➕"
                                font.pixelSize: 16
                                color: "#7B1FA2"
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            Text {
                                text: "添加成绩"
                                font.pixelSize: 14
                                font.weight: Font.Medium
                                color: "#7B1FA2"
                                anchors.verticalCenter: parent.verticalCenter
                            }

                        }

                    }

                    // 渐变背景
                    gradient: Gradient {
                        GradientStop {
                            position: 0
                            color: "#FAFBFF"
                        }

                        GradientStop {
                            position: 1
                            color: "#F0F4FF"
                        }

                    }

                }

            }

            layer.effect: DropShadow {
                horizontalOffset: 0
                verticalOffset: 4
                radius: 12
                samples: 25
                color: "#20000000"
            }

        }

    }

    // Error Dialog for validation messages
    Dialog {
        id: errorDialog

        property string errorMessage: ""

        title: "输入错误"
        modal: true
        standardButtons: Dialog.Ok
        anchors.centerIn: parent
        width: 320
        height: 200

        background: Rectangle {
            color: "white"
            radius: 12
            border.color: "#E0E0E0"
            border.width: 2
            layer.enabled: true

            layer.effect: DropShadow {
                horizontalOffset: 0
                verticalOffset: 4
                radius: 12
                samples: 25
                color: "#30000000"
            }

        }

        contentItem: Column {
            spacing: 16
            anchors.fill: parent
            anchors.margins: 20
            anchors.topMargin: 50

            // 图标和标题区域
            Row {
                spacing: 12
                anchors.horizontalCenter: parent.horizontalCenter

                Rectangle {
                    width: 40
                    height: 40
                    radius: 20
                    color: "#FFEBEE"
                    border.color: "#F44336"
                    border.width: 1

                    Text {
                        text: "❌"
                        font.pixelSize: 20
                        color: "#F44336"
                        anchors.centerIn: parent
                    }

                }

                Text {
                    text: "输入错误"
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    color: "#F44336"
                    anchors.verticalCenter: parent.verticalCenter
                }

            }

            // 错误信息
            Text {
                text: errorDialog.errorMessage
                font.pixelSize: 13
                wrapMode: Text.WordWrap
                width: parent.width - 10
                horizontalAlignment: Text.AlignHCenter
                color: "#424242"
                lineHeight: 1.3
                anchors.horizontalCenter: parent.horizontalCenter
            }

        }

    }

    // Add Score Dialog
    Dialog {
        id: addScoreDialog

        property string currentStudentId: ""
        property string currentStudentName: ""

        title: "为 " + currentStudentName + " 添加成绩"
        modal: true
        standardButtons: Dialog.Ok | Dialog.Cancel
        anchors.centerIn: parent
        width: 400
        onAccepted: {
            let studentId = currentStudentId;
            let s = scoreSubject.currentText; // 使用currentText而不是text
            let v = Number(scoreValue.text);
            let d = scoreDate.text.trim();
            let e = scoreExam.currentText;
            // 验证输入 - 使用弹窗提醒
            if (!studentId) {
                errorDialog.errorMessage = "学生ID不能为空，请重新选择学生";
                errorDialog.open();
                return ;
            }
            if (!s || scoreSubject.currentIndex < 0) {
                errorDialog.errorMessage = "请选择学科，学科不能为空";
                errorDialog.open();
                return ;
            }
            if (!scoreValue.text.trim()) {
                errorDialog.errorMessage = "成绩不能为空，请输入学生成绩";
                errorDialog.open();
                return ;
            }
            if (isNaN(v) || v < 0 || v > 150) {
                errorDialog.errorMessage = "成绩格式不正确\n请输入0-150之间的数字";
                errorDialog.open();
                return ;
            }
            if (!d) {
                errorDialog.errorMessage = "日期不能为空，请输入考试日期";
                errorDialog.open();
                return ;
            }
            if (!scoreDate.acceptableInput) {
                errorDialog.errorMessage = "日期格式不正确\n请输入正确的日期格式：YYYY-MM-DD\n例如：2025-03-15";
                errorDialog.open();
                return ;
            }
            // 验证日期有效性
            let datePattern = /^(\d{4})-(\d{2})-(\d{2})$/;
            let dateMatch = d.match(datePattern);
            if (!dateMatch) {
                errorDialog.errorMessage = "日期格式不正确\n请使用格式：YYYY-MM-DD\n例如：2025-03-15";
                errorDialog.open();
                return ;
            }
            let year = parseInt(dateMatch[1]);
            let month = parseInt(dateMatch[2]);
            let day = parseInt(dateMatch[3]);
            if (month < 1 || month > 12 || day < 1 || day > 31) {
                errorDialog.errorMessage = "日期值不正确\n月份应为01-12，日期应为01-31\n请检查输入的日期";
                errorDialog.open();
                return ;
            }
            // 验证考试类型
            if (!e || scoreExam.currentIndex < 0) {
                errorDialog.errorMessage = "请选择考试类型，考试类型不能为空";
                errorDialog.open();
                return ;
            }
            console.log("添加成绩：", s, v, d, e);
            // Find the student in studentsData and add score
            for (let i = 0; i < studentsData.length; i++) {
                if (studentsData[i].id === studentId) {
                    // Check if scoresData is a ListModel or JavaScript array
                    if (studentsData[i].scoresData && studentsData[i].scoresData.append) {
                        // It's a ListModel
                        studentsData[i].scoresData.append({
                            "subject": s,
                            "value": v,
                            "date": d,
                            "examType": e
                        });
                    } else {
                        // It's a JavaScript array
                        if (!studentsData[i].scoresData)
                            studentsData[i].scoresData = [];

                        studentsData[i].scoresData.push({
                            "subject": s,
                            "value": v,
                            "date": d,
                            "examType": e
                        });
                    }
                    break;
                }
            }
            // Update the corresponding model item
            for (let i = 0; i < studentModel.count; i++) {
                let modelItem = studentModel.get(i);
                if (modelItem.id === studentId) {
                    // Find the updated student from studentsData
                    let updatedStudent = studentsData.find((s) => {
                        return s.id === studentId;
                    });
                    if (updatedStudent)
                        studentModel.setProperty(i, "scoresData", updatedStudent.scoresData);

                    break;
                }
            }
            // Clear form fields
            scoreSubject.currentIndex = 0;
            scoreValue.text = "";
            scoreDate.text = "";
            scoreExam.currentIndex = 0;
            // Force refresh of the models to reflect changes
            studentModel.clear();
            for (let j = 0; j < studentsData.length; j++) {
                studentModel.append(studentsData[j]);
            }
            root.performSearch();
            // Close dialog
            addScoreDialog.close();
        }

        background: Rectangle {
            color: "white"
            radius: 8
        }

        contentItem: Column {
            spacing: 12
            padding: 16

            Text {
                text: "学生: " + addScoreDialog.currentStudentName
                font.bold: true
                font.pixelSize: 16
                color: "#333"
                width: parent.width - 32
            }

            ComboBox {
                id: scoreSubject

                width: parent.width - 32
                model: ["语文", "数学", "英语", "物理", "化学", "生物", "历史", "地理", "政治", "道法", "跨学科"]
                currentIndex: 0
                font.pixelSize: 14
                displayText: currentIndex >= 0 ? model[currentIndex] : "请选择学科"
            }

            TextField {
                id: scoreValue

                placeholderText: "成绩 (0-150)"
                width: parent.width - 32
                font.pixelSize: 14

                validator: IntValidator {
                    bottom: 0
                    top: 150
                }

            }

            Column {
                spacing: 2
                width: parent.width - 32

                TextField {
                    id: scoreDate

                    placeholderText: "日期 (YYYY-MM-DD)"
                    width: parent.width
                    font.pixelSize: 14

                    validator: RegularExpressionValidator {
                        regularExpression: /^\d{4}-\d{2}-\d{2}$/
                    }

                }

                // 输入提示 - 独立的位置避免被遮挡
                Text {
                    visible: scoreDate.text.length > 0 && !scoreDate.acceptableInput
                    text: "⚠️ 请输入正确的日期格式: YYYY-MM-DD"
                    color: "#F44336"
                    font.pixelSize: 11
                    font.weight: Font.Medium
                    width: parent.width
                    wrapMode: Text.WordWrap
                    leftPadding: 4
                }

            }

            ComboBox {
                id: scoreExam

                width: parent.width - 32
                model: ["期中考试", "期末考试", "月考", "周考", "随堂考"]
                currentIndex: 0
                font.pixelSize: 14
            }

        }

    }

    // Score Details Dialog
    Dialog {
        id: scoreDetailsDialog

        property string currentStudentId: ""
        property string currentStudentName: ""

        title: currentStudentName + " 的成绩详情"
        modal: true
        standardButtons: Dialog.Close
        anchors.centerIn: parent
        width: 600
        height: 500

        background: Rectangle {
            color: "white"
            radius: 8
            border.color: "#E0E0E0"
            border.width: 1
        }

        contentItem: ScrollView {
            anchors.fill: parent
            anchors.margins: 16

            Column {
                width: parent.width
                spacing: 12

                Text {
                    text: "学生: " + scoreDetailsDialog.currentStudentName
                    font.bold: true
                    font.pixelSize: 18
                    color: "#333"
                    width: parent.width
                }

                Rectangle {
                    width: parent.width
                    height: 2
                    color: "#E0E0E0"
                }

                Repeater {
                    model: scoreDetailsDialog.currentStudentId ? getSubjects(getStudentScores(scoreDetailsDialog.currentStudentId)) : []

                    delegate: Column {
                        width: parent.width
                        spacing: 8

                        Text {
                            text: "📚 " + modelData + " 科目"
                            font.bold: true
                            font.pixelSize: 16
                            color: "#1976D2"
                        }

                        Repeater {
                            model: {
                                if (!scoreDetailsDialog.currentStudentId)
                                    return [];

                                let scores = getStudentScores(scoreDetailsDialog.currentStudentId);
                                return scores ? scores.filter((score) => {
                                    return score.subject === modelData;
                                }) : [];
                            }

                            delegate: Rectangle {
                                width: parent.width
                                height: 60
                                radius: 8
                                color: "#F8F9FA"
                                border.color: "#E0E0E0"
                                border.width: 1

                                Row {
                                    anchors.fill: parent
                                    anchors.margins: 12
                                    spacing: 16

                                    Rectangle {
                                        width: 60
                                        height: 36
                                        radius: 18
                                        color: {
                                            let score = modelData.value;
                                            if (score >= 90)
                                                return "#4CAF50";

                                            if (score >= 80)
                                                return "#2196F3";

                                            if (score >= 70)
                                                return "#FF9800";

                                            return "#F44336";
                                        }
                                        anchors.verticalCenter: parent.verticalCenter

                                        Text {
                                            text: modelData.value
                                            color: "white"
                                            font.bold: true
                                            font.pixelSize: 16
                                            anchors.centerIn: parent
                                        }

                                    }

                                    Column {
                                        anchors.verticalCenter: parent.verticalCenter
                                        spacing: 4

                                        Text {
                                            text: "📅 " + modelData.date
                                            font.pixelSize: 14
                                            color: "#666"
                                        }

                                        Text {
                                            text: "📝 " + modelData.examType
                                            font.pixelSize: 12
                                            color: "#888"
                                        }

                                    }

                                }

                            }

                        }

                        Rectangle {
                            width: parent.width
                            height: 1
                            color: "#F0F0F0"
                        }

                    }

                }

            }

        }

    }

    filteredStudentModel: ListModel {
    }

    background: Rectangle {

        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#E8F5E9"
            }

            GradientStop {
                position: 1
                color: "#F1F8E9"
            }

        }

    }

    header: ToolBar {
        contentHeight: 60

        Item {
            anchors.fill: parent
            anchors.margins: 15

            Text {
                id: titleText

                text: "🎓 学生成绩管理页面"
                font.bold: true
                font.pixelSize: 20
                color: "white"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }

            Row {
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                spacing: 8

                Rectangle {
                    width: 200
                    height: 35
                    radius: 6
                    color: "white"
                    border.color: "#CBD5E1"
                    border.width: 1

                    TextField {
                        id: searchField

                        anchors.fill: parent
                        anchors.margins: 2
                        placeholderText: "搜索姓名、学校、年级、电话..."
                        font.pixelSize: 12
                        onTextChanged: {
                            root.searchText = text;
                        }
                        onAccepted: {
                            root.performSearch();
                        }

                        background: Rectangle {
                            color: "transparent"
                        }

                    }

                }

                Rectangle {
                    width: 70
                    height: 35
                    radius: 6
                    color: "#4CAF50"
                    border.color: "#2E7D32"
                    border.width: 1

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            root.performSearch();
                        }
                        onPressed: parent.color = "#388E3C"
                        onReleased: parent.color = "#4CAF50"
                    }

                    Row {
                        anchors.centerIn: parent
                        spacing: 4

                        Text {
                            text: "🔍"
                            font.pixelSize: 12
                            color: "white"
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Text {
                            text: "搜索"
                            font.pixelSize: 12
                            font.weight: Font.Medium
                            color: "white"
                            anchors.verticalCenter: parent.verticalCenter
                        }

                    }

                }

            }

        }

        background: Rectangle {
            anchors.fill: parent
            radius: 12

            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: "#4CAF50"
                }

                GradientStop {
                    position: 1
                    color: "#388E3C"
                }

            }

        }

    }

}
