#include <cassert>
#include <cmath>

#if defined(__has_include)
  #if __has_include(<glm/glm.hpp>)
    #include <glm/glm.hpp>
  #elif __has_include(<glm/vec3.hpp>)
    #include <glm/vec3.hpp>
  #else
    // Minimal fallback if GLM is not available (sufficient pour ce projet)
    namespace glm {
      struct vec3 {
        float x = 0.0f, y = 0.0f, z = 0.0f;
        vec3() = default;
        vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
        vec3& operator+=(const vec3& o) { x += o.x; y += o.y; z += o.z; return *this; }
      };
      inline vec3 operator+(const vec3& a, const vec3& b) { return vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
      inline vec3 operator-(const vec3& a, const vec3& b) { return vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
      inline vec3 operator*(const vec3& v, float s) { return vec3(v.x * s, v.y * s, v.z * s); }
      inline vec3 operator*(float s, const vec3& v) { return v * s; }
    }
  #endif
#else
  #include <glm/glm.hpp> // assume available
#endif

// AttractorSystem: base class for dynamical systems (header + implementation in one file)
class AttractorSystem
{
public:
    AttractorSystem();
    virtual ~AttractorSystem() = default;

    // Met à jour l'état en intégrant avec un pas fixe (mTimeStep).
    // Accumulation garantit la stabilité quand deltaTime est grand.
    void Update(float deltaTime);

    // Remet l'état courant à l'état initial.
    void Reset();

    // Définit l'état initial et remet l'état courant à cette valeur.
    void SetInitialState(const glm::vec3& state);

protected:
    // Chaque attracteur implémente le calcul de la dérivée dx/dt = f(x)
    virtual void ComputeDerivative(const glm::vec3& state, glm::vec3& outDerivative) = 0;

    // Méthodes d'intégration (utilisées par Update)
    void IntegrateEuler(float dt);
    void IntegrateRK4(float dt);

    glm::vec3 mCurrentState;
    glm::vec3 mInitialState;
    float mTimeStep;
    float mAccumulatedTime;
}; // <-- semicolon was missing previously

// Rössler attractor declaration
class RosslerAttractor : public AttractorSystem
{
public:
    RosslerAttractor(float a = 0.2f, float b = 0.2f, float c = 5.7f);

    // Implémentation de dx/dt = f(x)
    void ComputeDerivative(const glm::vec3& state, glm::vec3& outDerivative) override;

    // Accesseurs / mutateurs de paramètres
    void SetParameters(float a, float b, float c);
    void GetParameters(float& a, float& b, float& c) const;

private:
    float mA;
    float mB;
    float mC;
};

// Implementation: RosslerAttractor

RosslerAttractor::RosslerAttractor(float a, float b, float c)
    : mA(a), mB(b), mC(c)
{
    // Option : définir un état initial sensé pour Rössler
    SetInitialState(glm::vec3(0.1f, 0.0f, 0.0f));
    Reset();
}

void RosslerAttractor::ComputeDerivative(const glm::vec3& state, glm::vec3& outDerivative)
{
    // Rössler standard:
    // dx/dt = -y - z
    // dy/dt = x + a*y
    // dz/dt = b + z*(x - c)
    outDerivative.x = -state.y - state.z;
    outDerivative.y =  state.x + mA * state.y;
    outDerivative.z =  mB + state.z * (state.x - mC);
}

void RosslerAttractor::SetParameters(float a, float b, float c)
{
    mA = a; mB = b; mC = c;
}

void RosslerAttractor::GetParameters(float& a, float& b, float& c) const
{
    a = mA; b = mB; c = mC;
}

// Implementation: AttractorSystem

AttractorSystem::AttractorSystem()
    : mCurrentState(0.1f, 0.0f, 0.0f)
    , mInitialState(0.1f, 0.0f, 0.0f)
    , mTimeStep(0.01f)    // 10 ms par pas par défaut
    , mAccumulatedTime(0.0f)
{
}

void AttractorSystem::Update(float deltaTime)
{
    // Protéger des valeurs négatives et NaN
    if (deltaTime <= 0.0f || !std::isfinite(deltaTime)) return;

    mAccumulatedTime += deltaTime;

    // Intégration par pas fixe pour stabilité
    while (mAccumulatedTime >= mTimeStep) {
        // Choix de l'intégrateur : RK4 pour précision par défaut
        IntegrateRK4(mTimeStep);
        mAccumulatedTime -= mTimeStep;
    }
}

void AttractorSystem::Reset()
{
    mCurrentState = mInitialState;
    mAccumulatedTime = 0.0f;
}

void AttractorSystem::SetInitialState(const glm::vec3& state)
{
    mInitialState = state;
    mCurrentState = state;
}

void AttractorSystem::IntegrateEuler(float dt)
{
    glm::vec3 d;
    ComputeDerivative(mCurrentState, d);
    mCurrentState += d * dt;
}

void AttractorSystem::IntegrateRK4(float dt)
{
    glm::vec3 k1, k2, k3, k4;
    glm::vec3 tmp;

    ComputeDerivative(mCurrentState, k1);

    tmp = mCurrentState + (k1 * (dt * 0.5f));
    ComputeDerivative(tmp, k2);

    tmp = mCurrentState + (k2 * (dt * 0.5f));
    ComputeDerivative(tmp, k3);

    tmp = mCurrentState + (k3 * dt);
    ComputeDerivative(tmp, k4);

    mCurrentState += ( (k1 + 2.0f*k2 + 2.0f*k3 + k4) * (dt / 6.0f) );
}