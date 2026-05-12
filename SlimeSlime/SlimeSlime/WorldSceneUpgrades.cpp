#include "WorldScene.hpp"

void WorldScene::InitializeUpgradeContainer() {

    upgradeCost = 5;
    upgradeLevel = 1;
    damageLevel = 1;
    healthLevel = 1;
    rangeLevel = 1;
    cooldownLevel = 1;
    durabilityLevel = 1;
    trapDamageLevel = 1;
    trapCooldownLevel = 1;

    //open upgrade box button
    Button* openUpgrades = new Button(WIDTH - (WIDTH * 0.3), HEIGHT * 0.02, WIDTH * 0.29, HEIGHT * 0.1,
            { 90, 90, 90 }, { 220, 120, 0 }, { 70, 70, 70 }, { 200, 100, 0 }, 3, 
            [this]() { this->ToggleUpgradeBox(); }, 1.05);
    UI.push_back(openUpgrades);
    Text* upgradeLabel = new Text();
    upgradeLabel->Initialize("Upgrades", "../../fonts/PROXON.ttf", 30);
    openUpgrades->SetImage(upgradeLabel);

    //upgrade box
    //1% padding on edges of window
    upBox = new Container(WIDTH - (WIDTH * 0.3), HEIGHT * 0.14, WIDTH * 0.29, HEIGHT * 0.68,
        { 120, 120, 120 }, { 60, 60, 60 }, 120, 5);
    upBox->Toggle();
    int rowHeight = upBox->GetHeight() * 0.1;//gap of .025 allows for 8 buttons
    int buttonFontSize = upBox->GetWidth() * 0.15;
    int row = 0;
    Text* plusLabel = new Text();
    plusLabel->Initialize("+", "../../fonts/PROXON.ttf", buttonFontSize);
    textDrawSize = Vector2(upBox->GetWidth() * 0.7, rowHeight * 0.75);

    //cost label
    upgradeCostLabel = new Text();
    upgradeCostLabel->Initialize("Cost 5", "../../fonts/PROXON.ttf", 30);
    upgradeCostLabel->SetDrawSize(textDrawSize.x * 0.5, textDrawSize.y);
    upBox->AddText(upgradeCostLabel, upBox->GetWidth() * 0.05, (upBox->GetHeight() * 0.025) + (rowHeight * 0.125) + (rowHeight * row));

    SDL_Texture* coinTex = context.txm->LoadTexture(context.renderer, "../../assets/coin.png");
    Sprite* coinIcon = new Sprite();
    coinIcon->Initialize(coinTex, 2195, 2195, 0, 0, 35, 35);
    coinIcon->SetDrawSize(rowHeight * .75, rowHeight * .75);
    upBox->AddSprite(coinIcon, textDrawSize.x * 0.6, (upBox->GetHeight() * 0.025) + (rowHeight * 0.125) + (rowHeight * row));


    row++;
    //upgrade damage
      //button
    Button* upDamageButton = new Button(0, 0, rowHeight * .75, rowHeight * .75,
        { 90, 90, 90 }, { 220, 120, 0 }, { 70, 70, 70 }, { 200, 100, 0 }, 3,
        [this]() { this->UpgradeDamage(); }, 1.035);
    upDamageButton->SetImage(plusLabel);

      //label
    upDamageLabel = new Text();
    upDamageLabel->Initialize("Damage( 1 )", "../../fonts/PROXON.ttf", 100);
    upDamageLabel->SetDrawSize(textDrawSize.x, textDrawSize.y);
    upBox->AddText(upDamageLabel, upBox->GetWidth() * 0.05, (upBox->GetHeight() * 0.025) + (rowHeight * 0.125) + (rowHeight * row));
    upBox->AddButton(upDamageButton, upBox->GetWidth() * 0.7625, (upBox->GetHeight() * 0.025) + (rowHeight * 0.0625) + (rowHeight * row));


    //upgrade health
    row++;
      //button
    Button* upHealthButton = new Button(0, 0, rowHeight * .75, rowHeight * .75,
        { 90, 90, 90 }, { 220, 120, 0 }, { 70, 70, 70 }, { 200, 100, 0 }, 3,
        [this]() { this->UpgradeHealth(); }, 1.035);
    upHealthButton->SetImage(plusLabel);

    //label
    upHealthLabel = new Text();
    upHealthLabel->Initialize("Vitality( 1 )", "../../fonts/PROXON.ttf", 100);
    upHealthLabel->SetDrawSize(textDrawSize.x, textDrawSize.y);
    upBox->AddText(upHealthLabel, upBox->GetWidth() * 0.05, (upBox->GetHeight() * 0.025) + (rowHeight * 0.125) + (rowHeight * row));
    upBox->AddButton(upHealthButton, upBox->GetWidth() * 0.7625, (upBox->GetHeight() * 0.025) + (rowHeight * 0.0625) + (rowHeight * row));


    //upgrade attack speed
    row++;
      //button
    Button* upCooldownButton = new Button(0, 0, rowHeight * .75, rowHeight * .75,
        { 90, 90, 90 }, { 220, 120, 0 }, { 70, 70, 70 }, { 200, 100, 0 }, 3,
        [this]() { this->UpgradeAttackSpeed(); }, 1.035);
    upCooldownButton->SetImage(plusLabel);

    //label
    upCooldownLabel = new Text();
    upCooldownLabel->Initialize("Agility( 1 )", "../../fonts/PROXON.ttf", 100);
    upCooldownLabel->SetDrawSize(textDrawSize.x, textDrawSize.y);
    upBox->AddText(upCooldownLabel, upBox->GetWidth() * 0.05, (upBox->GetHeight() * 0.025) + (rowHeight * 0.125) + (rowHeight * row));
    upBox->AddButton(upCooldownButton, upBox->GetWidth() * 0.7625, (upBox->GetHeight() * 0.025) + (rowHeight * 0.0625) + (rowHeight * row));
    

    //upgrade attack range
    row++;
      //button
    Button* upRangeButton = new Button(0, 0, rowHeight * .75, rowHeight * .75,
        { 90, 90, 90 }, { 220, 120, 0 }, { 70, 70, 70 }, { 200, 100, 0 }, 3,
        [this]() { this->UpgradeAttackRange(); }, 1.035);
    upRangeButton->SetImage(plusLabel);

    //label
    upRangeLabel = new Text();
    upRangeLabel->Initialize("Atk. Range( 1 )", "../../fonts/PROXON.ttf", 100);
    upRangeLabel->SetDrawSize(textDrawSize.x, textDrawSize.y);
    upBox->AddText(upRangeLabel, upBox->GetWidth() * 0.05, (upBox->GetHeight() * 0.025) + (rowHeight * 0.125) + (rowHeight * row));
    upBox->AddButton(upRangeButton, upBox->GetWidth() * 0.7625, (upBox->GetHeight() * 0.025) + (rowHeight * 0.0625) + (rowHeight * row));
    

    //upgrade durability
    row++;
      //button
    Button* upDurabilityButton = new Button(0, 0, rowHeight * .75, rowHeight * .75,
        { 90, 90, 90 }, { 220, 120, 0 }, { 70, 70, 70 }, { 200, 100, 0 }, 3,
        [this]() { this->UpgradeStructureHealth(); }, 1.035);
    upDurabilityButton->SetImage(plusLabel);

    //label
    upDurabilityLabel = new Text();
    upDurabilityLabel->Initialize("Durability( 1 )", "../../fonts/PROXON.ttf", 100);
    upDurabilityLabel->SetDrawSize(textDrawSize.x, textDrawSize.y);
    upBox->AddText(upDurabilityLabel, upBox->GetWidth() * 0.05, (upBox->GetHeight() * 0.025) + (rowHeight * 0.125) + (rowHeight * row));
    upBox->AddButton(upDurabilityButton, upBox->GetWidth() * 0.7625, (upBox->GetHeight() * 0.025) + (rowHeight * 0.0625) + (rowHeight * row));
    

    //upgrade trap damage
    row++;
      //button
    Button* upTrapDamageButton = new Button(0, 0, rowHeight * .75, rowHeight * .75,
        { 90, 90, 90 }, { 220, 120, 0 }, { 70, 70, 70 }, { 200, 100, 0 }, 3,
        [this]() { this->UpgradeTrapDamage(); }, 1.035);
    upTrapDamageButton->SetImage(plusLabel);

    //label
    upTrapDamageLabel = new Text();
    upTrapDamageLabel->Initialize("Trap Damage( 1 )", "../../fonts/PROXON.ttf", 100);
    upTrapDamageLabel->SetDrawSize(textDrawSize.x, textDrawSize.y);
    upBox->AddText(upTrapDamageLabel, upBox->GetWidth() * 0.05, (upBox->GetHeight() * 0.025) + (rowHeight * 0.125) + (rowHeight * row));
    upBox->AddButton(upTrapDamageButton, upBox->GetWidth() * 0.7625, (upBox->GetHeight() * 0.025) + (rowHeight * 0.0625) + (rowHeight * row));
    
    
    //upgrade trap cooldown
    row++;
      //button
    Button* upTrapCooldownButton = new Button(0, 0, rowHeight * .75, rowHeight * .75,
        { 90, 90, 90 }, { 220, 120, 0 }, { 70, 70, 70 }, { 200, 100, 0 }, 3,
        [this]() { this->UpgradeTrapCooldown(); }, 1.035);
    upTrapCooldownButton->SetImage(plusLabel);

    //label
    upTrapCooldownLabel = new Text();
    upTrapCooldownLabel->Initialize("Trap Speed( 1 )", "../../fonts/PROXON.ttf", 100);
    upTrapCooldownLabel->SetDrawSize(textDrawSize.x, textDrawSize.y);
    upBox->AddText(upTrapCooldownLabel, upBox->GetWidth() * 0.05, (upBox->GetHeight() * 0.025) + (rowHeight * 0.125) + (rowHeight * row));
    upBox->AddButton(upTrapCooldownButton, upBox->GetWidth() * 0.7625, (upBox->GetHeight() * 0.025) + (rowHeight * 0.0625) + (rowHeight * row));
    
    
    UI.push_back(upBox);
}

void WorldScene::UpgradeDamage() {

    if (!CanUpgrade()) return;
    player->RemoveCoins(upgradeCost);
    AttackCone* cone = player->GetAttackCone();
    cone->IncreaseAttackDamage(cone->GetAttackDamage() * 0.1);

    damageLevel++;
    upDamageLabel->SetText("Damage( " + to_string(damageLevel) + " )");
    upDamageLabel->SetDrawSize(textDrawSize.x, textDrawSize.y);
    IncreaseUpgradeCost();
}

void WorldScene::UpgradeHealth() {

    if (!CanUpgrade()) return;
    player->RemoveCoins(upgradeCost);
    player->IncreaseMaxHealth(player->GetHealth() * 0.1);
    player->DecreaseHealCooldownByPercent(0.02);

    healthLevel++;
    upHealthLabel->SetText("Vitality( " + to_string(healthLevel) + " )");
    upHealthLabel->SetDrawSize(textDrawSize.x, textDrawSize.y);

    IncreaseUpgradeCost();
}

void WorldScene::UpgradeAttackSpeed() {

    if (!CanUpgrade()) return;
    player->RemoveCoins(upgradeCost);
    AttackCone* cone = player->GetAttackCone();
    cone->DecreaseAttackCooldown(cone->GetCooldownTime() * 0.05);
    player->SetMovementSpeed(player->GetMovementSpeed() * 0.05);

    cooldownLevel++;
    upCooldownLabel->SetText("Agility( " + to_string(cooldownLevel) + " )");
    upCooldownLabel->SetDrawSize(textDrawSize.x, textDrawSize.y);

    IncreaseUpgradeCost();
}

void WorldScene::UpgradeAttackRange() {

    if (!CanUpgrade()) return;
    player->RemoveCoins(upgradeCost);
    AttackCone* cone = player->GetAttackCone();
    cone->IncreaseWidth(cone->GetHalfAngle() * 0.04);
    cone->IncreaseRadius(cone->GetRadius() * 0.02);

    rangeLevel++;
    upRangeLabel->SetText("Atk. Range( " + to_string(rangeLevel) + " )");
    upRangeLabel->SetDrawSize(textDrawSize.x, textDrawSize.y);

    IncreaseUpgradeCost();
}

void WorldScene::UpgradeStructureHealth() {

    if (!CanUpgrade()) return;
    player->RemoveCoins(upgradeCost);

    wallHstone->SetDurability(wallHstone->GetMaxHealth() * 1.1);
    wallHwood->SetDurability(wallHwood->GetMaxHealth() * 1.1);
    wallVstone->SetDurability(wallVstone->GetMaxHealth() * 1.05);
    wallVwood->SetDurability(wallVwood->GetMaxHealth() * 1.05);
    pushTrap->SetDurability(pushTrap->GetMaxHealth() * 1.05);
    spikeTrap->SetDurability(spikeTrap->GetMaxHealth() * 1.05);
    fireTrap->SetDurability(fireTrap->GetMaxHealth() * 1.05);
    freezeTrap->SetDurability(freezeTrap->GetMaxHealth() * 1.05);
    explosionTrap->SetDurability(explosionTrap->GetMaxHealth() * 1.05);

    vector<Structure*> structures = context.grid->GetNearbyStructures(context.grid->WorldToGrid(context.grid->GetAtlas()->GetPosition()), context.grid->GetGridWidth());
    for (Structure* s : structures) {
        s->SetDurability(s->GetMaxHealth() * 1.05);
    }

    durabilityLevel++;
    upDurabilityLabel->SetText("Durability( " + to_string(durabilityLevel) + " )");
    upDurabilityLabel->SetDrawSize(textDrawSize.x, textDrawSize.y);

    IncreaseUpgradeCost();
}

void WorldScene::UpgradeTrapDamage() {

    if (!CanUpgrade()) return;
    player->RemoveCoins(upgradeCost);

    pushTrap->IncreaseAttackByPercent(0.1);
    spikeTrap->IncreaseAttackByPercent(0.1);
    fireTrap->IncreaseAttackByPercent(0.1);
    freezeTrap->IncreaseAttackByPercent(0.1);
    explosionTrap->IncreaseAttackByPercent(0.1);

    trapDamageLevel++;
    upTrapDamageLabel->SetText("Trap Damage( " + to_string(trapDamageLevel) + " )");
    upTrapDamageLabel->SetDrawSize(textDrawSize.x, textDrawSize.y);

    IncreaseUpgradeCost();
}

void WorldScene::UpgradeTrapCooldown() {

    if (!CanUpgrade()) return;
    player->RemoveCoins(upgradeCost);

    pushTrap->DecreaseTrapCooldownByPercent(0.1);
    spikeTrap->DecreaseTrapCooldownByPercent(0.1);
    fireTrap->DecreaseTrapCooldownByPercent(0.1);
    freezeTrap->DecreaseTrapCooldownByPercent(0.1);
    explosionTrap->DecreaseTrapCooldownByPercent(0.1);

    trapCooldownLevel++;
    upTrapCooldownLabel->SetText("Trap\n Cooldown( " + to_string(trapCooldownLevel) + " )");
    upTrapCooldownLabel->SetDrawSize(textDrawSize.x, textDrawSize.y);

    IncreaseUpgradeCost();
}

bool WorldScene::CanUpgrade() {
	return player->HasEnoughCoins(upgradeCost);
}

void WorldScene::IncreaseUpgradeCost() {
    upgradeLevel++;
    
    //0.075x^2 + 5
    upgradeCost = roundf(0.075 * (upgradeLevel * upgradeLevel)) + 5;
    upgradeCostLabel->SetText("Cost " + to_string(upgradeCost));
    upgradeCostLabel->SetDrawSize(textDrawSize.x * 0.5, textDrawSize.y);

    context.am->PlaySound("Upgrade", "Default", { player->GetPosition().x, 100, player->GetPosition().y }, { 0, 0, 0 }, Vector2(1, 1));
}

void WorldScene::UpdateUpgradeLabelColor() {
    if (CanUpgrade())
        upgradeCostLabel->SetColor({ 255, 255, 255 });
    else
        upgradeCostLabel->SetColor({ 255, 50, 50 });

}

void WorldScene::ToggleUpgradeBox() {
    upBox->Toggle();

    //play sound
    context.am->PlaySound("BuildMode", "Default", { player->GetPosition().x, 100, player->GetPosition().y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
}