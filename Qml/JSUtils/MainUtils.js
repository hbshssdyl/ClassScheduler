.pragma library
function getMinClassInfoWidth(index)
{
    switch(index)
    {
        case 0://序号
            return 40;
        case 1://日期
            return 80;
        case 2://星期
            return 60;
        case 3://姓名
            return 60;
        case 4://学校
            return 100;
        case 5://电话
            return 90;
        case 6://年级
            return 40;
        case 7://学科
            return 50;
        case 8://时间
            return 80;
        case 9://老师
            return 80;
        case 10://网课or面授
            return 80;
        case 11://课时
            return 40;
        case 12://金额/小时
            return 70;
        case 13://课酬总计
            return 70;
        case 14://老师姓名
            return 60;
        case 15://老师工资
            return 60;
        case 16://已收金额
            return 60;
        case 17://付费方式
            return 60;
        case 18://收费日期
            return 80;
        default:
            return 100;
    }
}

function getClassInfoWidth(windowWidth, index, count){
    //40 80 60 60 100 100 40 40 80 80 80 40 80 80 60 60 60 60 80
    //"日期", "星期", "姓名", "学校", "电话", "年级", "学科", "时间", "老师", "网课or面授", "课时", "金额/小时", "课酬总计", "老师姓名", "老师工资", "已收金额", "付费方式", "收费日期"
    var sum = 0;
    for(var i=0; i<count; i++)
    {
        sum += getMinClassInfoWidth(i);
    }

    if(windowWidth > sum)
    {
        return getMinClassInfoWidth(index) + (windowWidth - sum) / count;
    }
    return getMinClassInfoWidth(index);
}

function getMinTeacherInfoWidth(index)
{
    switch(index)
    {
        case 0://序号
            return 40;
        case 1://老师姓名
            return 60;
        case 2://使用过的昵称
            return 200;
        case 3://教过的科目级学生
            return 300;
        case 4://科目及工资（每小时）
            return 200;
        case 5://科目及年级
            return 200;
        default:
            return 200;
    }
}

function getTeacherInfoWidth(windowWidth, index, count){
    //"老师姓名", "使用过的昵称", "科目及工资（每小时）", "科目及年级"
    var sum = 0;
    for(var i=0; i<count; i++)
    {
        sum += getMinTeacherInfoWidth(i);
    }

    if(windowWidth > sum)
    {
        return getMinTeacherInfoWidth(index) + (windowWidth - sum) / count;
    }
    return getMinTeacherInfoWidth(index);
}

function getMinStudentInfoWidth(index)
{
    switch(index)
    {
        case 0://序号
            return 40;
        case 1://学生姓名
            return 60;
        case 2://就读学校
            return 150;
        case 3://手机号
            return 150;
        case 4://教过该生的老师
            return 250;
        case 5://科目及费用
            return 250;
        default:
            return 200;
    }
}

function getStudentInfoWidth(windowWidth, index, count){
    //"学生姓名", "就读学校", "手机号", "教过该生的老师", "科目及费用"
    var sum = 0;
    for(var i=0; i<count; i++)
    {
        sum += getMinStudentInfoWidth(i);
    }

    if(windowWidth > sum)
    {
        return getMinStudentInfoWidth(index) + (windowWidth - sum) / count;
    }
    return getMinStudentInfoWidth(index);
}

function getMinToolsWidth(index)
{
    switch(index)
    {
        case 0://显示总课程数
            return 152;
        case 1://显示各科课程数
            return 140;
        case 2://显示成绩曲线
            return 140;
        default:
            return 150;
    }
}

function getToolsItemWidth(windowWidth, index, count){

    var sum = 0;
    for(var i=0; i<count; i++)
    {
        sum += getMinToolsWidth(i);
    }

    if(windowWidth > sum)
    {
        return getMinToolsWidth(index) + (windowWidth - sum) / count;
    }
    return getMinToolsWidth(index);
}
