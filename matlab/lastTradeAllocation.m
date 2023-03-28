function allocation = lastTradeAllocation(qty, side, af, netPosAcct, scenario)

nAccts = length(af);

switch scenario
    
    case '1'
        
        allocation = abs(netPosAcct);
        netPosAcct = netPosAcct + side*allocation;
        if sum(abs(netPosAcct)) ~= 0
            disp('something is not right');
        end
        
    case '2'
        
        par0 = af*qty;
        par = round(par0);
        
        while (sum(par) ~= qty)
            if sum(par) > qty
                index = find(par0 == min(par0));
                if par(index(1)) > 0
                    par(index(1)) = par(index(1))-1;
                end
            else
                index = find(par0 == max(par0));
                par(index(1)) = par(index(1))+1;
            end
        end
        
        allocation = par;
        
    case '4'
        
        par0 = af*qty;
        par = round(par0);
        
        while (sum(par) ~=  qty)
            if sum(par) > qty
                index = find(par == max(par));
                par(index(1)) = par(index(1))-1;
            else
                index = find(par == max(par));
                par(index(1)) = par(index(1))+1;
            end
        end

        netPosAcct2 = netPosAcct + side*par;
        mae2 = criterionNetPosition(netPosAcct2, af);
        minMae = mae2;
        allocation = par;
        for i = 1:nAccts
            for j = 1:nAccts
                if i ~= j
                    actionVector = zeros(1, nAccts);
                    actionVector(i) = +1;
                    actionVector(j) = -1;
                    par1 = par + actionVector;
                    if sum(par1<0)>0
                        %ignore this scenario
                        par1 = par;
                    end
                    netPosAcct2 = netPosAcct + side*par1;
                    mae2 = criterionNetPosition(netPosAcct2, af);
                    if mae2 < minMae
                        minMae = mae2;
                        allocation = par1;
                    end
                    
                end
            end
        end

    case '3'
        
        netPosSystem = sum(netPosAcct) + side*qty;
        sgnNet = sign(netPosSystem);
        par = af*abs(netPosSystem);
        par = round(par);
        
        while (sum(par) ~= abs(netPosSystem))
            
            if sum(par) > abs(netPosSystem)
                index = find(par == max(par));
                par(index(1)) = par(index(1))-1;
            else
                index = find(par == max(par));
                par(index(1)) = par(index(1))+1;
            end
            
        end
        
        netPosAcct2 = sgnNet*par;
        
        %qtAcct = netPosAcct2 - netPosAcct;
        %parSet.optimalAllocation = abs(qtAcct);
        
        netPosAcct2 = sanityCheckOnNetPosAcct(netPosAcct, netPosAcct2, side, qty);
        
        qtAcct = netPosAcct2 - netPosAcct;
        allocation = abs(qtAcct);

end