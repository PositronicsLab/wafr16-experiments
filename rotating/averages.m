% Computes the mean time between contact manifold changes vs. mean adaptive
% integration step

% manchanges: the differences between changes to the contact manifold (all
%             values should be non-negative since time increases monotonically)
% intsteps: the sequence of integration step sizes taken (all values should be
%           strictly positive)
% maxstep:  the maximum integration step

function [avgmanchange, avgintstep] = averages(manchanges, intsteps, maxstep)

  % get the average time between manifold changes; we do this processing
  % so that if there is a large time between a contact manifold change- say
  % 1.0 seconds- and the maximum integration step is 0.4, the time between
  % the contact manifold change is treated as [ 0.4 0.4 0.2 ] to be able to
  % compare against the mean integration steps 
  aug = [];
  for i=1:length(manchanges)
    if (manchanges(i) > maxstep)
      aug(length(aug)+1) = maxstep;
      manchanges(i) = manchanges(i) - maxstep;
      i = i - 1;
    end
  end
  manchanges = [manchanges; aug'];

  avgmanchange = mean(manchanges);
  avgintstep = mean(intsteps);

