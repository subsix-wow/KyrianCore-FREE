UPDATE battlepay_display_info SET Name1='10,000 gold coins', Name3='Buy 10,000 pieces of gold!' WHERE DisplayInfoId='78';
UPDATE battlepay_display_info SET Name1='50,000 gold coins', Name3='Buy 50,000 pieces of gold!' WHERE DisplayInfoId='79';
UPDATE battlepay_display_info SET Name1='200,000 gold coins', Name3='Buy 200,000 pieces of gold!' WHERE DisplayInfoId='80';
UPDATE battlepay_display_info SET Name1='500,000 gold coins', Name3='Buy 500,000 pieces of gold!' WHERE DisplayInfoId='81';

DELETE FROM battlepay_display_info WHERE DisplayInfoId='77';

UPDATE battlepay_display_info SET Name1='Change in appearance', Name3='Change the look of your character!' WHERE DisplayInfoId='76';

UPDATE battlepay_display_info SET Name1='Change in race', Name3='Change the race of your character!' WHERE DisplayInfoId='75';
UPDATE battlepay_display_info SET Name1='Change in faction', Name3='Change the faction of your character!' WHERE DisplayInfoId='74';
UPDATE battlepay_display_info SET Name1='Change in name', Name3='Change the name of your character!' WHERE DisplayInfoId='73';

UPDATE battlepay_display_info SET Name1='Boost - Level 50', Name3='Take your character to level 50!' WHERE DisplayInfoId='72';

UPDATE battlepay_display_info SET Name1='Soul of Aspects' WHERE DisplayInfoId='100';

-- INSERT INTO quest_offer_reward('ID','RewardText') VALUES 
-- (30039, "$bToday is the day when your fate will be decided. Do you have the intellect and force of will necessary to be a $c? Or are you destined to a life scribing in the dusty libraries?$b$bEach has its place, but I can sense that you aspire to greatness.$b$bLet us begin our training, my newest pupil."); 
