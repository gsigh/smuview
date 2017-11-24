/*
 * This file is part of the SmuView project.
 *
 * Copyright (C) 2017 Frank Stettner <frank-stettner@gmx.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SESSION_HPP
#define SESSION_HPP

#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

#include <QObject>
#include <QSettings>

using std::function;
using std::mutex;
using std::shared_ptr;
using std::string;
using std::unordered_set;
using std::vector;

namespace sigrok {
class Context;
class Session;
}

namespace sv {

class DeviceManager;

namespace data {
class SignalBase;
}

namespace devices {
class Device;
class HardwareDevice;
}

class Session : public QObject
{
    Q_OBJECT

public:
	enum capture_state {
		Stopped,
		AwaitingTrigger,
		Running
	};

	static shared_ptr<sigrok::Context> sr_context;

public:
	Session(DeviceManager &device_manager);
	~Session();

	DeviceManager& device_manager();
	const DeviceManager& device_manager() const;

	//shared_ptr<sigrok::Session> session() const;

	void save_settings(QSettings &settings) const;
	void restore_settings(QSettings &settings);

	void add_device(shared_ptr<devices::HardwareDevice> device);
	void remove_device(shared_ptr<devices::HardwareDevice> device);

	void add_signal(shared_ptr<data::SignalBase> signal);

	void load_init_file(const string &file_name, const string &format);

	capture_state get_capture_state() const;
	void start_capture(function<void (const QString)> error_handler);
	void stop_capture();

private:
	void set_capture_state(capture_state state);

	DeviceManager &device_manager_;
	shared_ptr<sigrok::Session> sr_session_;
	vector<shared_ptr<devices::Device>> devices_;

	std::thread sampling_thread_;
	mutable mutex sampling_mutex_; //!< Protects access to capture_state_.
	unordered_set<shared_ptr<data::SignalBase>> all_signals_;
	capture_state capture_state_;
	bool out_of_memory_;

	void sample_thread_proc(function<void (const QString)> error_handler);
	void free_unused_memory();

Q_SIGNALS:
	void capture_state_changed(int state);

};

} // namespace sv

#endif // SESSION_HPP
