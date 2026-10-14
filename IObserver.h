#pragma once
class IObserver
{
public:
    virtual ~IObserver() = default;
    virtual void onCameraChanged() = 0;
};

