%HAHP

criteriaOption = '1';
sanityOnNetPosition = 0;

filePath = '.';
accountName = 'SAUMA';

%{
managedAccounts = {
    'Sauma' '00000001'  'MS'       0.42
    'Sauma' '00000002'  'MS'       0.30
    'Sauma' '00000003'  'MS'       0.15
    'Sauma' '00000004'  'MS'       0.10
    'Sauma' '00000005'  'MS'       0.03
    };
%}


managedAccounts = {
    'Sauma' '00000001'  'MS'       0.53
    'Sauma' '00000002'  'MS'       0.17
    'Sauma' '00000003'  'MS'       0.14
    'Sauma' '00000004'  'MS'       0.09
    'Sauma' '00000005'  'MS'       0.06
    'Sauma' '00000006'  'MS'       0.01
    };
%}
%managedAccounts2 = managedAccounts(:,:);

nAccts = size(managedAccounts,1);
sum1 = 0;
for i=1:nAccts
    sum1 = sum1+managedAccounts{i,end};
end
if (sum1 ~= 1)
    disp('something is wrong');
end



% Bund
symbol1 = 'crudeoil';
%symbol2 = 'FGBLZ1';
symbol2 = 'crudeoil';
rollDate = [];

%whichAction = 'sort';
whichAction = 'expand';
%whichAction = 'contract';
% whichAction = 'default';

whichOrder = 1;
%whichOrder = [-1 0];
postTradeAllocation(managedAccounts, symbol1, symbol2, rollDate, filePath, accountName, whichAction, criteriaOption, sanityOnNetPosition, whichOrder);

% %% APS
% 
% criteriaOption = '1';
% sanityOnNetPosition = 0;
% managedAccounts2 = managedAccounts([1 2 3 4],:);
% 
% % UST
% symbol1 = 'ZNZ1';
% symbol2 = 'ZNZ1';
% rollDate = [];
% whichAction = 'expand';
% whichOrder = [0 1 -1];
% postTradeAllocation(managedAccounts2, symbol1, symbol2, rollDate, whichAction, criteriaOption, sanityOnNetPosition, whichOrder);
% 
% % Mini
% symbol1 = 'ESZ1';
% symbol2 = 'ESZ1';
% rollDate = [];
% whichAction = 'expand';
% whichOrder = [1 -1 -1];
% postTradeAllocation(managedAccounts2, symbol1, symbol2, rollDate, whichAction, criteriaOption, sanityOnNetPosition, whichOrder);
% 
% % Crude Oil
% symbol1 = 'CLZ1';
% symbol2 = 'CLZ1';
% rollDate = [];
% whichAction = 'expand';
% whichOrder = [-1 0 -1];
% postTradeAllocation(managedAccounts2, symbol1, symbol2, rollDate, whichAction, criteriaOption, sanityOnNetPosition, whichOrder);
