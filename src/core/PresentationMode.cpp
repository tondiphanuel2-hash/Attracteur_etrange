/**
 * PresentationMode.cpp - Implémentation du mode présentation
 * 
 * Contrôle automatique de la démonstration.
 */

#include "PresentationMode.h"
#include "../graphics/Camera3D.h"
#include <iostream>

/**
 * PresentationMode - Constructeur
 */
PresentationMode::PresentationMode()
    : mConfig()
    , mIsActive(false)
    , mCurrentPhase(PresentationPhase::LORENZ_INTRO)
    , mPhaseTimer(0.0f)
    , mTotalTimer(0.0f)
    , mInitialCameraDistance(50.0f)
    , mTargetCameraDistance(50.0f)
{
}

/**
 * Start - Démarre la présentation
 */
void PresentationMode::Start()
{
    mIsActive = true;
    mCurrentPhase = PresentationPhase::LORENZ_INTRO;
    mPhaseTimer = 0.0f;
    mTotalTimer = 0.0f;
    
    std::cout << "🎬 Présentation démarrée" << std::endl;
}

/**
 * Stop - Arrête la présentation
 */
void PresentationMode::Stop()
{
    mIsActive = false;
    std::cout << "⏹️  Présentation arrêtée" << std::endl;
}

/**
 * Update - Met à jour la présentation
 * 
 * Return: Index attracteur à charger (-1 si pas de changement)
 */
int PresentationMode::Update(Camera3D* camera, float deltaTime)
{
    if (!mIsActive || !camera)
    {
        return -1;
    }
    
    mPhaseTimer += deltaTime;
    mTotalTimer += deltaTime;
    
    int attractorChange = -1;
    
    // Mettre à jour selon la phase actuelle
    switch (mCurrentPhase)
    {
        case PresentationPhase::LORENZ_INTRO:
            UpdateLorenzIntro(camera, deltaTime);
            if (mPhaseTimer >= mConfig.phaseDuration)
            {
                AdvanceToNextPhase();
            }
            break;
            
        case PresentationPhase::LORENZ_ROTATE:
            UpdateLorenzRotate(camera, deltaTime);
            if (mPhaseTimer >= mConfig.phaseDuration)
            {
                AdvanceToNextPhase();
            }
            break;
            
        case PresentationPhase::TRANSITION_TO_ROSSLER:
            attractorChange = UpdateTransitionToRossler(camera, deltaTime);
            if (mPhaseTimer >= mConfig.transitionDuration)
            {
                AdvanceToNextPhase();
            }
            break;
            
        case PresentationPhase::ROSSLER_SHOWCASE:
            UpdateRosslerShowcase(camera, deltaTime);
            if (mPhaseTimer >= mConfig.phaseDuration)
            {
                AdvanceToNextPhase();
            }
            break;
            
        case PresentationPhase::TRANSITION_TO_CHEN:
            attractorChange = UpdateTransitionToChen(camera, deltaTime);
            if (mPhaseTimer >= mConfig.transitionDuration)
            {
                AdvanceToNextPhase();
            }
            break;
            
        case PresentationPhase::CHEN_SHOWCASE:
            UpdateChenShowcase(camera, deltaTime);
            if (mPhaseTimer >= mConfig.phaseDuration)
            {
                AdvanceToNextPhase();
            }
            break;
            
        case PresentationPhase::FINALE:
            attractorChange = UpdateFinale(camera, deltaTime);
            if (mPhaseTimer >= mConfig.phaseDuration)
            {
                AdvanceToNextPhase();
            }
            break;
            
        case PresentationPhase::COMPLETED:
            if (mConfig.loopPresentation)
            {
                Start();  // Recommencer
                attractorChange = 0;  // Retour à Lorenz
            }
            else
            {
                Stop();
            }
            break;
    }
    
    return attractorChange;
}

/**
 * UpdateLorenzIntro - Phase d'introduction
 */
void PresentationMode::UpdateLorenzIntro(Camera3D* camera, float deltaTime)
{
    // Zoom progressif vers l'attracteur
    float progress = mPhaseTimer / mConfig.phaseDuration;
    
    if (mPhaseTimer < 0.1f)
    {
        mInitialCameraDistance = camera->GetDistance();
        mTargetCameraDistance = 60.0f;
    }
    
    // Interpolation douce (ease-in-out)
    float t = progress * progress * (3.0f - 2.0f * progress);
    float distance = mInitialCameraDistance + (mTargetCameraDistance - mInitialCameraDistance) * t;
    camera->SetDistance(distance);
}

/**
 * UpdateLorenzRotate - Rotation autour de Lorenz
 */
void PresentationMode::UpdateLorenzRotate(Camera3D* camera, float deltaTime)
{
    // Rotation lente et continue
    camera->Rotate(mConfig.rotationSpeed * deltaTime * 100.0f, 0.0f);
}

/**
 * UpdateTransitionToRossler - Transition vers Rössler
 */
int PresentationMode::UpdateTransitionToRossler(Camera3D* camera, float deltaTime)
{
    // Zoom out rapide
    if (mPhaseTimer < 0.1f)
    {
        mInitialCameraDistance = camera->GetDistance();
        mTargetCameraDistance = 80.0f;
    }
    
    float progress = mPhaseTimer / mConfig.transitionDuration;
    float distance = mInitialCameraDistance + (mTargetCameraDistance - mInitialCameraDistance) * progress;
    camera->SetDistance(distance);
    
    // Charger Rössler au début de la transition
    if (mPhaseTimer < 0.1f)
    {
        return 1;  // Rössler
    }
    
    return -1;
}

/**
 * UpdateRosslerShowcase - Présentation de Rössler
 */
void PresentationMode::UpdateRosslerShowcase(Camera3D* camera, float deltaTime)
{
    // Rotation plus rapide pour Rössler (plus compact)
    camera->Rotate(mConfig.rotationSpeed * deltaTime * 150.0f, 
                   mConfig.rotationSpeed * deltaTime * 20.0f);
    
    // Zoom in progressif
    if (mPhaseTimer < mConfig.phaseDuration * 0.5f)
    {
        camera->Zoom(mConfig.zoomSpeed * deltaTime);
    }
}

/**
 * UpdateTransitionToChen - Transition vers Chen
 */
int PresentationMode::UpdateTransitionToChen(Camera3D* camera, float deltaTime)
{
    // Reset distance
    if (mPhaseTimer < 0.1f)
    {
        camera->SetDistance(70.0f);
        return 2;  // Chen
    }
    
    return -1;
}

/**
 * UpdateChenShowcase - Présentation de Chen
 */
void PresentationMode::UpdateChenShowcase(Camera3D* camera, float deltaTime)
{
    // Rotation variée pour Chen (double hélice)
    float angle = mPhaseTimer * 0.5f;
    camera->Rotate(mConfig.rotationSpeed * deltaTime * 100.0f * std::cos(angle),
                   mConfig.rotationSpeed * deltaTime * 50.0f * std::sin(angle));
}

/**
 * UpdateFinale - Phase finale
 */
int PresentationMode::UpdateFinale(Camera3D* camera, float deltaTime)
{
    // Retour à Lorenz avec zoom out spectaculaire
    if (mPhaseTimer < 0.1f)
    {
        mInitialCameraDistance = camera->GetDistance();
        mTargetCameraDistance = 120.0f;
        return 0;  // Lorenz
    }
    
    float progress = mPhaseTimer / mConfig.phaseDuration;
    float distance = mInitialCameraDistance + (mTargetCameraDistance - mInitialCameraDistance) * progress;
    camera->SetDistance(distance);
    
    // Rotation finale
    camera->Rotate(mConfig.rotationSpeed * deltaTime * 200.0f, 0.0f);
    
    return -1;
}

/**
 * AdvanceToNextPhase - Passe à la phase suivante
 */
void PresentationMode::AdvanceToNextPhase()
{
    mPhaseTimer = 0.0f;
    
    switch (mCurrentPhase)
    {
        case PresentationPhase::LORENZ_INTRO:
            mCurrentPhase = PresentationPhase::LORENZ_ROTATE;
            std::cout << "📍 Phase: Rotation Lorenz" << std::endl;
            break;
        case PresentationPhase::LORENZ_ROTATE:
            mCurrentPhase = PresentationPhase::TRANSITION_TO_ROSSLER;
            std::cout << "📍 Phase: Transition → Rössler" << std::endl;
            break;
        case PresentationPhase::TRANSITION_TO_ROSSLER:
            mCurrentPhase = PresentationPhase::ROSSLER_SHOWCASE;
            std::cout << "📍 Phase: Présentation Rössler" << std::endl;
            break;
        case PresentationPhase::ROSSLER_SHOWCASE:
            mCurrentPhase = PresentationPhase::TRANSITION_TO_CHEN;
            std::cout << "📍 Phase: Transition → Chen" << std::endl;
            break;
        case PresentationPhase::TRANSITION_TO_CHEN:
            mCurrentPhase = PresentationPhase::CHEN_SHOWCASE;
            std::cout << "📍 Phase: Présentation Chen" << std::endl;
            break;
        case PresentationPhase::CHEN_SHOWCASE:
            mCurrentPhase = PresentationPhase::FINALE;
            std::cout << "📍 Phase: Finale" << std::endl;
            break;
        case PresentationPhase::FINALE:
            mCurrentPhase = PresentationPhase::COMPLETED;
            std::cout << "✅ Présentation terminée" << std::endl;
            break;
        case PresentationPhase::COMPLETED:
            // Déjà terminé
            break;
    }
}

/**
 * GetPhaseName - Nom de la phase actuelle
 */
std::string PresentationMode::GetPhaseName() const
{
    switch (mCurrentPhase)
    {
        case PresentationPhase::LORENZ_INTRO:
            return "Introduction - Lorenz";
        case PresentationPhase::LORENZ_ROTATE:
            return "Rotation - Lorenz";
        case PresentationPhase::TRANSITION_TO_ROSSLER:
            return "Transition → Rössler";
        case PresentationPhase::ROSSLER_SHOWCASE:
            return "Présentation - Rössler";
        case PresentationPhase::TRANSITION_TO_CHEN:
            return "Transition → Chen";
        case PresentationPhase::CHEN_SHOWCASE:
            return "Présentation - Chen";
        case PresentationPhase::FINALE:
            return "Finale - Retour Lorenz";
        case PresentationPhase::COMPLETED:
            return "Terminé";
        default:
            return "Inconnu";
    }
}

/**
 * GetProgress - Progression dans la phase
 */
float PresentationMode::GetProgress() const
{
    float duration = mConfig.phaseDuration;
    
    if (mCurrentPhase == PresentationPhase::TRANSITION_TO_ROSSLER ||
        mCurrentPhase == PresentationPhase::TRANSITION_TO_CHEN)
    {
        duration = mConfig.transitionDuration;
    }
    
    return std::min(mPhaseTimer / duration, 1.0f);
}

/**
 * GetTotalProgress - Progression totale
 */
float PresentationMode::GetTotalProgress() const
{
    // Durée totale estimée
    float totalDuration = mConfig.phaseDuration * 5.0f +  // 5 phases normales
                         mConfig.transitionDuration * 2.0f; // 2 transitions
    
    return std::min(mTotalTimer / totalDuration, 1.0f);
}