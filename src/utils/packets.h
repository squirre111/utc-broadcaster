#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <cstring>


/**
   Абстрактный пакет отправляемый по сети
*/
class Packet {

public:
	virtual const uint8_t *Data() const = 0;
	virtual size_t Size() const = 0;
    virtual void SetData(const void *data, size_t len) = 0;
};


/**
   Пакет фиксированного размера, выделяет память на стеке
*/
template<int N>
class FixedPacket : public Packet {
	
public:
	const uint8_t *Data() const override { return data_; }
	size_t Size() const override { return N; }
    
    void SetData(const void *data, size_t len) override {
        if (len == N) memcpy(data_, data, N);
    }
	
private:
	uint8_t data_[N];
};


/**
   Пакет произвольного размера, выделяет память в куче

   Представлен для демонстрации
*/
class DynamicPacket : public Packet {
	
public:
	DynamicPacket() : data_(nullptr), size_(0) {}
	
	const uint8_t *Data() const override { return data_; }
	size_t Size() const override { return size_; }
    
    void SetData(const void *data, size_t len) override {
        free(data_);
        data_ = static_cast<uint8_t *>(calloc(1, len));
        memcpy(data_, data, len);
        size_ = len;
    }
    ~DynamicPacket() {
        free(data_);
    }
	
private:
	uint8_t *data_;
	size_t size_;
};
