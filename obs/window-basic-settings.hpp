/******************************************************************************
    Copyright (C) 2013 by Hugh Bailey <obs.jim@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#pragma once

#include <util/util.hpp>
#include <QDialog>
#include <memory>

#include <obs.h>

class OBSBasic;
class QAbstractButton;
class QComboBox;
class OBSPropertiesView;

#include "ui_OBSBasicSettings.h"

class OBSBasicSettings : public QDialog {
	Q_OBJECT

private:
	OBSBasic *main;

	std::unique_ptr<Ui::OBSBasicSettings> ui;
	bool generalChanged = false;
	bool outputsChanged = false;
	bool audioChanged = false;
	bool videoChanged = false;
	int  pageIndex = 0;
	bool loading = true;

	OBSPropertiesView *streamProperties = nullptr;
	OBSPropertiesView *streamEncoderProps = nullptr;
	OBSPropertiesView *recordEncoderProps = nullptr;

	void SaveCombo(QComboBox *widget, const char *section,
			const char *value);
	void SaveComboData(QComboBox *widget, const char *section,
			const char *value);
	void SaveCheckBox(QAbstractButton *widget, const char *section,
			const char *value, bool invert = false);
	void SaveEdit(QLineEdit *widget, const char *section,
			const char *value);
	void SaveSpinBox(QSpinBox *widget, const char *section,
			const char *value);

	inline bool Changed() const
	{
		return generalChanged || outputsChanged ||
			audioChanged || videoChanged;
	}

	inline void EnableApplyButton(bool en)
	{
		ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(en);
	}

	inline void ClearChanged()
	{
		generalChanged = false;
		outputsChanged = false;
		audioChanged   = false;
		videoChanged   = false;
		EnableApplyButton(false);
	}

	void HookWidget(QWidget *widget, const char *signal, const char *slot);

	bool QueryChanges();

	void LoadServiceTypes();
	void LoadServiceInfo();
	void LoadEncoderTypes();

	void LoadGeneralSettings();
	void LoadOutputSettings();
	void LoadAudioSettings();
	void LoadVideoSettings();
	void LoadSettings(bool changedOnly);

	OBSPropertiesView *CreateEncoderPropertyView(const char *encoder,
			const char *path, bool changed = false);

	/* general */
	void LoadLanguageList();

	/* output */
	void LoadSimpleOutputSettings();
	void LoadAdvOutputStreamingSettings();
	void LoadAdvOutputStreamingEncoderProperties();
	void LoadAdvOutputRecordingSettings();
	void LoadAdvOutputRecordingEncoderProperties();
	void LoadAdvOutputFFmpegSettings();
	void LoadAdvOutputAudioSettings();

	/* audio */
	void LoadListValues(QComboBox *widget, obs_property_t *prop,
		const char *configName);
	void LoadAudioDevices();

	/* video */
	void LoadRendererList();
	void ResetDownscales(uint32_t cx, uint32_t cy);
	void LoadDownscaleFilters();
	void LoadResolutionLists();
	void LoadFPSData();

	void SaveGeneralSettings();
	void SaveOutputSettings();
	void SaveAudioSettings();
	void SaveVideoSettings();
	void SaveSettings();

private slots:
	void on_simpleOutUseBufsize_toggled(bool checked);
	void on_simpleOutputVBitrate_valueChanged(int val);

	void on_listWidget_itemSelectionChanged();
	void on_buttonBox_clicked(QAbstractButton *button);

	void on_streamType_currentIndexChanged(int idx);
	void on_simpleOutputBrowse_clicked();
	void on_advOutRecPathBrowse_clicked();
	void on_advOutFFPathBrowse_clicked();
	void on_advOutEncoder_currentIndexChanged(int idx);
	void on_advOutRecEncoder_currentIndexChanged(int idx);

	void on_baseResolution_editTextChanged(const QString &text);

	void GeneralChanged();
	void AudioChanged();
	void AudioChangedRestart();
	void OutputsChanged();
	void VideoChanged();
	void VideoChangedResolution();
	void VideoChangedRestart();

protected:
	virtual void closeEvent(QCloseEvent *event);

public:
	OBSBasicSettings(QWidget *parent);
};
