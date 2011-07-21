/*

Copyright (C) University of Oxford, 2005-2011

University of Oxford means the Chancellor, Masters and Scholars of the
University of Oxford, having an administrative office at Wellington
Square, Oxford OX1 2JD, UK.

This file is part of Chaste.

Chaste is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 2.1 of the License, or
(at your option) any later version.

Chaste is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details. The offer of Chaste under the terms of the
License is subject to the License being interpreted in accordance with
English Law and subject to any action against the University of Oxford
being under the jurisdiction of the English Courts.

You should have received a copy of the GNU Lesser General Public License
along with Chaste. If not, see <http://www.gnu.org/licenses/>.

*/

#include "VolumeConstraintUpdateRule.hpp"

template<unsigned DIM>
VolumeConstraintUpdateRule<DIM>::VolumeConstraintUpdateRule()
    : AbstractPottsUpdateRule<DIM>(),
      mDeformationEnergyParameter(0.5), // Educated guess
      mMatureCellTargetVolume(16.0) // Defaults to a 4*4 cell size
{
}

template<unsigned DIM>
VolumeConstraintUpdateRule<DIM>::~VolumeConstraintUpdateRule()
{
}

template<unsigned DIM>
double VolumeConstraintUpdateRule<DIM>::EvaluateHamiltonianContribution(unsigned currentNodeIndex,
                                                                        unsigned targetNodeIndex,
                                                                        PottsBasedCellPopulation& rCellPopulation)
{
	double delta_H = 0.0;

	// This method only works in 2D at present
	assert(DIM == 2);

	std::set<unsigned> containing_elements = rCellPopulation.GetNode(currentNodeIndex)->rGetContainingElementIndices();
	std::set<unsigned> new_location_containing_elements = rCellPopulation.GetNode(targetNodeIndex)->rGetContainingElementIndices();

    bool current_node_contained = !containing_elements.empty();
    bool target_node_contained = !new_location_containing_elements.empty();

    // At least one of the current node and target node must be in an element
    assert(current_node_contained || target_node_contained);

    // Every node must each be in at most one element
    assert(new_location_containing_elements.size() < 2);

    // The current node and target node must not be in the same element
    assert(new_location_containing_elements.begin() != containing_elements.begin());

	if (current_node_contained) // current node is in an element
	{
		unsigned current_element = (*containing_elements.begin());
		double current_volume = rCellPopulation.rGetMesh().GetVolumeOfElement(current_element);
		double current_volume_difference = current_volume - mMatureCellTargetVolume;

		delta_H += mDeformationEnergyParameter*((current_volume_difference + 1.0)*(current_volume_difference + 1.0) - current_volume_difference*current_volume_difference);
	}
	if (target_node_contained) // target node is in an element
	{
		unsigned target_element = (*new_location_containing_elements.begin());
        double target_volume = rCellPopulation.rGetMesh().GetVolumeOfElement(target_element);
        double target_volume_difference = target_volume - mMatureCellTargetVolume;

		delta_H += mDeformationEnergyParameter*((target_volume_difference - 1.0)*(target_volume_difference - 1.0) - target_volume_difference*target_volume_difference);
	}

	return delta_H;
}

template<unsigned DIM>
double VolumeConstraintUpdateRule<DIM>::GetDeformationEnergyParameter()
{
    return mDeformationEnergyParameter;
}

template<unsigned DIM>
void VolumeConstraintUpdateRule<DIM>::SetDeformationEnergyParameter(double deformationEnergyParameter)
{
    mDeformationEnergyParameter = deformationEnergyParameter;
}

template<unsigned DIM>
double VolumeConstraintUpdateRule<DIM>::GetMatureCellTargetVolume() const
{
    return mMatureCellTargetVolume;
}

template<unsigned DIM>
void VolumeConstraintUpdateRule<DIM>::SetMatureCellTargetVolume(double matureCellTargetVolume)
{
    assert(matureCellTargetVolume >= 0.0);
    mMatureCellTargetVolume = matureCellTargetVolume;
}

template<unsigned DIM>
void VolumeConstraintUpdateRule<DIM>::OutputUpdateRuleParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<DeformationEnergyParameter>" << mDeformationEnergyParameter << "</DeformationEnergyParameter> \n";
	*rParamsFile << "\t\t\t<MatureCellTargetVolume>" << mMatureCellTargetVolume << "</MatureCellTargetVolume> \n";

    // Call method on direct parent class
    AbstractPottsUpdateRule<DIM>::OutputUpdateRuleParameters(rParamsFile);
}

/////////////////////////////////////////////////////////////////////////////
// Explicit instantiation
/////////////////////////////////////////////////////////////////////////////

template class VolumeConstraintUpdateRule<1>;
template class VolumeConstraintUpdateRule<2>;
template class VolumeConstraintUpdateRule<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(VolumeConstraintUpdateRule)
