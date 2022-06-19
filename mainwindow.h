#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <QColorDialog>
#include <QSlider>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

signals:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

public:
    explicit MainWindow(QWidget *parent = nullptr,
                        int t = 0,
                        bool isAnimated = true);
    ~MainWindow();

private slots:
    void changeAnimationState();

private:
    void connectSignalsAndSlots();

    std::unique_ptr<Ui::MainWindow> ui;
    int t;
    bool isAnimated;
    std::unique_ptr<QColorDialog> colorDialog;
};

#endif /** MAINWINDOW_H */
