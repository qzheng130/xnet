#ifndef XNET_BASE_NONCOPYABLE_H
#define XNET_BASE_NONCOPYABLE_H

namespace xnet
{

class noncopyable
{
public:
    noncopyable(const noncopyable &) = delete;
    void operator=(const noncopyable &) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

} // namespace xnet

#endif // XNET_BASE_NONCOPYABLE_H
