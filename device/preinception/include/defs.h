#ifndef DEVICE_DEFS_H
#define DEVICE_DEFS_H

enum class TYPE : uint32_t {
    REQUEST = 0,
    RESPONSE = 1
};

enum class CMD : uint32_t {
    HANDSHAKE = 1,
    ADD = 2,
    MULP2 = 3,
    DIVP2 = 4
};

enum class ADD : uint32_t {
    OP_1 = 0,
    OP_2 = 1,
    OP_3 = 2
};

enum class MULP2 : uint32_t {
    OP_1 = 0,
    OP_2 = 1,
    OP_3 = 2
};

enum class DIVP2 : uint32_t {
    OP_1 = 0,
    OP_2 = 1,
    OP_3 = 2
};

enum class HANDSHAKE : uint32_t {
    OP = 0
};

union DevicePayload {

    struct Fields {
        char id_and_type;
        char cmd_and_sub_cmd;
        char body_1;
        char body_2;
        char body_3;
        char body_4;
    } fields;

    struct Fields32 {
        uint16_t header;
        uint32_t body;
    } fields32;

    char packet[6];

    DevicePayload() 
    {
        fields.id_and_type = 0;
        fields.cmd_and_sub_cmd = 0;
        fields.body_1 = 0;
        fields.body_2 = 0;
        fields.body_3 = 0;
        fields.body_4 = 0;
    }

    uint32_t id() { return (fields.id_and_type & 15); };
    void id(uint32_t id) { fields.id_and_type = (id & 15) | ((fields.id_and_type >> 4) << 4); };

    uint32_t type() { return (fields.id_and_type & (15 << 4)) >> 4; };
    void type(uint32_t type) { fields.id_and_type = ((type & 15) << 4) | (fields.id_and_type & 15); };

    uint32_t cmd() { return (fields.cmd_and_sub_cmd & 15); };
    void cmd(uint32_t cmd) { fields.cmd_and_sub_cmd = (cmd & 15) | ((fields.cmd_and_sub_cmd >> 4) << 4); };

    uint32_t sub_cmd() { return (fields.cmd_and_sub_cmd & (15 << 4)) >> 4; };
    void sub_cmd(uint32_t sub_cmd) { fields.cmd_and_sub_cmd = ((sub_cmd & 15) << 4) | (fields.cmd_and_sub_cmd & 15); };

    std::string print()
    {
        std::stringstream ss;

        ss << "[DevicePayload]" << std::endl;
        ss << "\tID: " << std::to_string(id()) << std::endl;
        ss << "\tType: " << std::to_string(type()) << std::endl;
        ss << "\tCMD: " << std::to_string(cmd()) << std::endl;
        ss << "\tSUB_CMD: " << std::to_string(sub_cmd()) << std::endl;
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
    void for_each(Function f) {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        for (auto& item : list_) {
            f(item);
        }
    }
};

#endif