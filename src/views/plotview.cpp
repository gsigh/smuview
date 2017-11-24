/*
 * This file is part of the SmuView project.
 *
 * Copyright (C) 2017 Frank Stettner <frank-stettner@gmx.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <QBoxLayout>
#include <QVBoxLayout>

#include "plotview.hpp"
#include "src/session.hpp"
#include "src/data/analog.hpp"
#include "src/data/curvedata.hpp"
#include "src/widgets/plot.hpp"

namespace sv {
namespace views {

PlotView::PlotView(Session &session,
	shared_ptr<data::Analog> x_signal_data,
	shared_ptr<data::Analog> y_signal_data,
	QWidget *parent) :
		BaseView(session, parent),
	x_signal_data_(x_signal_data),
	y_signal_data_(y_signal_data)
{
	setup_ui();
	connect_signals();
	init_values();

	plot->start();
}

void PlotView::setup_ui()
{
	QVBoxLayout *layout = new QVBoxLayout(this);

	data::CurveData *curve_data = new data::CurveData(
		x_signal_data_, y_signal_data_);

	plot = new widgets::Plot(curve_data, this);
	plot->set_plot_mode(widgets::Plot::PlotModes::Additive);
	plot->set_plot_interval(200); // 200ms

	layout->addWidget(plot);
}

void PlotView::connect_signals()
{
}

void PlotView::init_values()
{
}

} // namespace views
} // namespace sv
