/* -*- c++ -*- */
/*
 * Gqrx SDR: Software defined radio receiver powered by GNU Radio and Qt
 *           https://gqrx.dk/
 *
 * Copyright 2014 Alexandru Csete OZ9AEC.
 *
 * Gqrx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Gqrx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Gqrx; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */
#ifndef IQ_TOOL_H
#define IQ_TOOL_H

#include <QCloseEvent>
#include <QDialog>
#include <QDir>
#include <QPalette>
#include <QSettings>
#include <QShowEvent>
#include <QString>
#include <QTimer>

namespace Ui {
    class CIqTool;
}


struct iqt_cplx
{
    float re;
    float im;
};


/*! \brief User interface for I/Q recording and playback. */
class CIqTool : public QDialog
{
    Q_OBJECT

public:
    explicit CIqTool(QWidget *parent = 0);
    ~CIqTool();

    void setSampleRate(qint64 sr);

    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent * event);

    void saveSettings(QSettings *settings);
    void readSettings(QSettings *settings);

signals:
    void startRecording(const QString recdir);
    void stopRecording();
    void startPlayback(const QString filename, float samprate, qint64 center_freq, bool repeat);
    void stopPlayback();
    void seek(qint64 seek_pos);

public slots:
    void cancelRecording();
    void cancelPlayback();

private slots:
    void on_recDirEdit_textChanged(const QString &text);
    void on_recDirButton_clicked();
    void on_recButton_clicked(bool checked);
    void on_playButton_clicked(bool checked);
    void on_slider_valueChanged(int value);
    void on_listWidget_currentTextChanged(const QString &currentText);
    void on_centerFreqSpinBox_valueChanged(double value);
    void on_sampleRateSpinBox_valueChanged(double value);
    void timeoutFunction(void);

private:
    enum IqToolState
    {
        STATE_IDLE = 0,
        STATE_PLAYING,
        STATE_RECORDING
    };
    void refreshDir(void);
    void refreshTimeWidgets(void);
    void parseFileName(const QString &filename);
    void switchControlsState(enum IqToolState state);
    void setPlaybackSampleRate(qint64 sr);

private:
    Ui::CIqTool *ui;

    QDir        *recdir;
    QTimer      *timer;
    QPalette    *error_palette; /*!< Palette used to indicate an error. */

    QString current_file;      /*!< Selected file in file browser. */

    bool    is_recording;
    bool    is_playing;
    int     bytes_per_sample;  /*!< Bytes per sample (fc = 4) */
    int     playback_sample_rate;       /*!< Playback sample rate. */
    int     recording_sample_rate;      /*!< Recording sample rate. */
    qint64  center_freq;       /*!< Center frequency. */
    int     rec_len;           /*!< Length of a recording in seconds */
};

#endif // IQ_TOOL_H
