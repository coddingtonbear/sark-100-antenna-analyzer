#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "config.h"
#include "version.h"
#include "eventreceiver.h"
#include "seriallink.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public EventReceiver
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void RaiseEvent(event_t event, int arg);

    static const Version version;

    SerialLink *link;

private:
    void set_band(double f, double span);
    void set_scan_disp();
    void draw_graph1();
    void populate_table();
    void toDom(QDomDocument &doc);
    void fromDom(QDomElement &e0);

    Ui::MainWindow *ui;

private slots:
    void Slot_scanBtn_click();
    void Slot_scanDummyBtn_click();
    void Slot_cursor_move(double pos);
    void Slot_band_change(int idx);
    void Slot_fcentre_change(double v);
    void Slot_fspan_change(double v);
    void Slot_point_count_change(int);
    void Slot_plot_change(int);
    void Slot_menuDevice_Show();
    void Slot_menuDevice_Select(QAction *act);
    void Slot_Load();
    void Slot_Save();
    void Slot_Settings();
    void Slot_about();
    void Slot_copy();
};

#endif // MAINWINDOW_H
