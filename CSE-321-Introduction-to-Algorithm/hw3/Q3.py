def findMinEnergy(current_position, remaining_parts, energy_cost):
    if not remaining_parts:  # All parts assembled
        return energy_cost
    
    min_energy = 99999999999
    for part in remaining_parts:
        new_energy_cost = energy_cost + energyCost(current_position, part)
        updated_remaining_parts = remaining_parts - part
        
        # Recursive call for each possible next part
        energy = findMinEnergy(part, updated_remaining_parts, new_energy_cost)
        
        min_energy = min(min_energy, energy)
    
    return min_energy

# Usage example
parts = {1, 2, 3, 4, 5}
print(findMinEnergy(0, parts, 0))
