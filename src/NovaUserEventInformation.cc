#include "NovaUserEventInformation.hh"


NovaUserEventInformation::NovaUserEventInformation()
    : hitCount(0),photonCountScintillation(0),photonCountCherenkov(0),absorptionCount(0),boundaryAbsorptionCount(0),
      outOfWorldCount(0),totalEnergyDeposition(0.),energyWeightedPosition(0.),reconstructionPosition(0.),
      positionMax(0.),energyDepositionMax(0.), pmtCountAboveThreshold(0) {}

NovaUserEventInformation::~NovaUserEventInformation() {}
