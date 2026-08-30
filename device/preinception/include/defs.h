#ifndef DEVICE_DEFS_H
#define DEVICE_DEFS_H

union DevicePayload {

    struct Fields {
        char id;
        char type;
        char cmd;
        char sub_cmd;
        char body_1;
        char body_2;
        char body_3;
        char body_4;
    } fields;

    struct Fields32 {
        uint32_t header;
        uint32_t body;
    } fields32;

    char packet[8];

    std::string print()
    {
        std::stringstream ss;

        ss << "[DevicePayload]" << std::endl;
        ss << "\tID: " << std::to_string(fields.id) << std::endl;
        ss << "\tType: " << std::to_string(fields.type) << std::endl;
        ss << "\tCMD: " << std::to_string(fields.cmd) << std::endl;
        ss << "\tSUB_CMD: " << std::to_string(fields.sub_cmd) << std::endl;
        ss << "\tBODY: " << std::to_string(fields32.body);

        return ss.str();
    }
};

template<typename T>
class ThreadSafeList {
private:
    std::list<T> list_;
    // mutable allows locking in const methods (like size() or contains())
    mutable std::shared_mutex mutex_; 

public:
    ThreadSafeList() = default;
    ~ThreadSafeList() = default;

    // Delete copy and move semantics to prevent accidental unsafe copies
    ThreadSafeList(const ThreadSafeList&) = delete;
    ThreadSafeList& operator=(const ThreadSafeList&) = delete;

    // --- Write Operations (Exclusive Lock) ---

    void push_back(const T& value) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        list_.push_back(value);
    }

    void push_front(const T& value) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        list_.push_front(value);
    }

    // Returns std::optional to prevent TOCTOU (Time-Of-Check to Time-Of-Use) race conditions
    std::optional<T> pop_back() {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        if (list_.empty()) {
            return std::nullopt;
        }
        T val = std::move(list_.back());
        list_.pop_back();
        return val;
    }

    std::optional<T> pop_front() {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        if (list_.empty()) {
            return std::nullopt;
        }
        T val = std::move(list_.front());
        list_.pop_front();
        return val;
    }

    void remove(const T& value) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        list_.remove(value);
    }

    // --- Read Operations (Shared Lock) ---

    bool empty() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return list_.empty();
    }

    size_t size() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return list_.size();
    }

    // Safe iteration: apply a function to all elements while holding the lock
    template<typename Function>
    void for_each(Function f) const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        for (const auto& item : list_) {
            f(item);
        }
    }
};

#endif