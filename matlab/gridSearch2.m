function [optimalAllocation parSet] = gridSearch2(cumPnl, netPosSystem, netPosAcct, af, qt, pnlAcctSoFar, pricePre, priceNow, onePtValue, scale, criteriaOption, sanityOnNetPosition)

sgnNet = sign(netPosSystem);
par  = af*abs(netPosSystem);
par  = round(par);

% sanity check
% there are many different ways of doing this
% floor all and then add to those with biggest remainder
% until you get the original sum back
% at the end the method is indifferent
while (sum(par) ~= abs(netPosSystem))
    if sum(par) > abs(netPosSystem)
        index = find(par == max(par));
        if length(index)>1
            index = find(par./af == max(par./af));            
        end
        par(index(1)) = par(index(1))-1;
        %par(index(end)) = par(index(end))-1;
    else
        index = find(par == max(par));
        if length(index)>1
            index = find(par./af == max(par./af));            
        end
        par(index(1)) = par(index(1))+1;
        %par(index(end)) = par(index(end))+1;
    %disp(index)
    end
end

netPosAcct2 = sgnNet*par;
qtAcct = netPosAcct2 - netPosAcct;
parSet.optimalAllocation = abs(qtAcct);

% fprintf("Net position ");
% for j=1:6
%     fprintf('%d ',netPosAcct(j));
% end
% 
% fprintf(" Before: ");
% for j=1:6
%     fprintf('%d ',netPosAcct2(j));
% end
% fprintf('\n');

netPosAcct2 = sanityCheckOnNetPosAcct(netPosAcct, netPosAcct2, scale, qt);
% 
% fprintf('After: ');
% for j=1:6
%     fprintf('%d ',netPosAcct2(j));
% end
% fprintf('\n');



qtAcct = netPosAcct2 - netPosAcct;
parSet.optimalAllocation = abs(qtAcct);

if (scale < 0 && sum(qtAcct>0)>0) || (scale > 0 && sum(qtAcct<0)>0) || sum(qtAcct) ~= scale*qt
    disp('ERROR: In gridSreach2 - something is not right ...');
end

parSet.netPosAcct2 = netPosAcct2;
parSet.sgnNet = sgnNet;
parSet.af = af;
parSet.cumPnl = cumPnl;
parSet.pnlAcctSoFar = pnlAcctSoFar;
parSet.netPosAcct = netPosAcct;
parSet.scale = scale;
parSet.qt = qt;
parSet.pricePre = pricePre;
parSet.priceNow = priceNow;
parSet.onePtValue = onePtValue;
parSet.criteriaOption = criteriaOption;
parSet.sanityOnNetPosition = sanityOnNetPosition;
       
pnlAcctSoFar2 = pnlAcctSoFar + netPosAcct2*(priceNow-pricePre)*onePtValue;
parSet.minMae = criterion(pnlAcctSoFar2, cumPnl, af, '1');

parSet.optimalCase = [];
parSet.netSoFar = netPosAcct2;

parSet.perturbVec = [+1 -1];
parSet = optimizingPnlByPerturbingNetPos(parSet);

parSet.perturbVec = [+1 -1 +1 -1];
parSet = optimizingPnlByPerturbingNetPos(parSet);

parSet.perturbVec = [-1 -1 +2];
parSet = optimizingPnlByPerturbingNetPos(parSet);

parSet.perturbVec = [+1 +1 -2];
parSet = optimizingPnlByPerturbingNetPos(parSet);

parSet.perturbVec = [+2 -2];
parSet = optimizingPnlByPerturbingNetPos(parSet);

parSet.perturbVec = [+3 -3];
parSet = optimizingPnlByPerturbingNetPos(parSet);

parSet.perturbVec = [+1 +2 -3];
parSet = optimizingPnlByPerturbingNetPos(parSet);

parSet.perturbVec =	[+3 -1 -2];
parSet = optimizingPnlByPerturbingNetPos(parSet);

optimalAllocation = parSet.optimalAllocation;
%

