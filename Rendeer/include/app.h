#pragma once

namespace rendeer
{
    class App
    {
    public:
        App() {}
        ~App() {}

        virtual void Initialize() {}
        virtual void Shutdown() {}
        virtual void Update(float) {}
        virtual void Render() {}
    private:
        
    };
} // namespace rendeer
