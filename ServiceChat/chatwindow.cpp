﻿#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "tim_msg_c.h"
#include "tim_c.h"
#include "Tim/timtool.h"
#include <QColorDialog>
#include <QDebug>
#include <QFileDialog>
#include <QScrollBar>
#include <QWaitCondition>

#ifdef _WIN32
#include <Windows.h>
#endif

#include <ctime>
ChatWindow::ChatWindow(const Linkman &linkman, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    otherId = linkman.id;
    otherNick = !linkman.nick.trimmed().isEmpty() ? linkman.nick : otherId;
    otherRemark = !linkman.remark.trimmed().isEmpty() ? linkman.remark : otherNick;
    TimTool::Instance().AddChatWindowMap(otherId, this);
    setWindowTitle(tr("%1 - Session").arg(otherRemark));
    GetConversation();
    if(!TimTool::Instance().GetContentEX(otherId).isEmpty())
    {
        auto list = TimTool::Instance().GetContentEX(otherId);
        for(auto s : list)
        {
            AddContent(otherId, otherNick, s.time, s.text);
        }
    }
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::AddContent(QString id, QString nick, time_t time, QString msg)
{
    QPalette pal = ui->textBrowser->palette();
    QColor c;
    pal.setColor(QPalette::Base, c.blue());
    std::tm *p_tm = std::localtime(&time);
    QString str_time = QString("%1-%2 %3:%4:%5").
            arg(p_tm->tm_mon + 1, 2, 10, QChar('0')).
            arg(p_tm->tm_mday, 2, 10, QChar('0')).
            arg(p_tm->tm_hour, 2, 10, QChar('0')).
            arg(p_tm->tm_min, 2, 10, QChar('0')).
            arg(p_tm->tm_sec, 2, 10, QChar('0'));

    ui->textBrowser->append(QString(R"(
                                    <font color="blue">%1(%2) %3</font>
                                    )").arg(id).arg(nick).arg(str_time));
    pal.setColor(QPalette::Base, c.black());
    ui->textBrowser->append(msg);
    ui->textBrowser->append("");

    QScrollBar *verticalScrollBar = ui->textBrowser->verticalScrollBar();
    if(verticalScrollBar)
    {
        verticalScrollBar->setSliderPosition(verticalScrollBar->maximum());
    }

    QScrollBar *horizontalScrollBar = ui->textBrowser->horizontalScrollBar();
    if(horizontalScrollBar)
    {
        horizontalScrollBar->setSliderPosition(verticalScrollBar->minimum());
    }
}

void ChatWindow::GetConversation()
{
    if(TimTool::Instance().ContainInConvMap(otherId))
    {
        convHandle = TimTool::Instance().GetConvHandle(otherId);
        return;
    }
    QByteArray bytes = otherId.toUtf8();
    convHandle = CreateConversation();
    int rt = TIMGetConversation(convHandle, kCnvC2C, bytes.data());
    if(rt == 0)
    {
        qDebug() << "GetConversation Success!";
        TimTool::Instance().AddConvMap(otherId, convHandle);
        emit TimTool::Instance().NewConversation(otherId, otherNick, GetTime());
    }
    else
        qDebug() << "GetConversation Error!";
}

void ChatWindow::closeEvent(QCloseEvent *event)
{
    TimTool::Instance().RemoveChatWindowMap(otherId);
    TimTool::Instance().RemoveConvMap(otherId);
    DestroyConversation(convHandle);
}

void ChatWindow::on_sendBtn_clicked()
{
    QString text = ui->textEdit->toHtml();
    qDebug() << text;
    TimTool::Instance().SendMsg(otherId, text);
    AddContent(TimTool::Instance().getId(), TimTool::Instance().getNick(), GetTime(), text);
    ui->textEdit->clear();
}

void ChatWindow::on_closeBtn_clicked()
{
    ui->textEdit->append(R"(
                         <img src = "D:\keys\201995-120HG1030762.jpg" />
                         )");
}

//void ChatWindow::on_selectPicBtn_clicked()
//{
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Images (*.jpg *.xpm *.png);;"));
//    if(!fileName.isNull())
//    {
//        QString html = QString(R"(
//                               <img src = "%1" />
//                               )").arg(fileName);
//        ui->textEdit->append(html);
//    }
//}

void ChatWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
    ui->textEdit->setCurrentFont(f);
    ui->textEdit->setFocus();
}

void ChatWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->textEdit->setFontPointSize(arg1.toDouble());
    ui->textEdit->setFocus();
}

void ChatWindow::on_boldToolButton_clicked(bool checked)
{
    if(checked)
    {
        ui->textEdit->setFontWeight(QFont::Bold);
    }
    else
    {
        ui->textEdit->setFontWeight(QFont::Normal);
    }
    ui->textEdit->setFocus();
}

void ChatWindow::on_italicToolButton_clicked(bool checked)
{
    ui->textEdit->setFontItalic(checked);
    ui->textEdit->setFocus();
}

void ChatWindow::on_lineToolButton_clicked(bool checked)
{
    ui->textEdit->setFontUnderline(checked);
    ui->textEdit->setFocus();
}

void ChatWindow::on_colorToolButton_clicked(bool checked)
{
    auto color = QColorDialog::getColor();
    if(color.isValid())
    {
        ui->textEdit->setTextColor(color);
        ui->textEdit->setFocus();
    }
}
