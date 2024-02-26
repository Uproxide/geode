#pragma once

#include <Geode/ui/General.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/ui/TextArea.hpp>
#include "ModItem.hpp"
#include "ModListSource.hpp"

using namespace geode::prelude;

struct ModListErrorStatus {};
struct ModListUnkProgressStatus {};
struct ModListProgressStatus {
    uint8_t percentage;
};
using ModListStatus = std::variant<ModListErrorStatus, ModListUnkProgressStatus, ModListProgressStatus>;

class ModList : public CCNode, public SetTextPopupDelegate {
protected:
    ModListSource* m_source;
    size_t m_page = 0;
    ScrollLayer* m_list;
    CCMenu* m_statusContainer;
    CCLabelBMFont* m_statusTitle;
    SimpleTextArea* m_statusDetails;
    CCMenuItemSpriteExtra* m_statusDetailsBtn;
    CCSprite* m_statusLoadingCircle;
    Slider* m_statusLoadingBar;
    ModListSource::PageLoadEventListener m_listener;
    CCMenuItemSpriteExtra* m_pagePrevBtn;
    CCMenuItemSpriteExtra* m_pageNextBtn;
    CCMenuItemSpriteExtra* m_pageLabelBtn;
    CCLabelBMFont* m_pageLabel;

    bool init(ModListSource* src, CCSize const& size);

    void onPromise(ModListSource::PageLoadEvent* event);
    void onPage(CCObject*);
    void onGoToPage(CCObject*);
    void onShowStatusDetails(CCObject*);

    void setTextPopupClosed(SetTextPopup*, gd::string value) override;

    void updatePageUI(bool hide = false);

public:
    static ModList* create(ModListSource* src, CCSize const& size);

    void reloadPage();
    void gotoPage(size_t page, bool update = false);
    void showStatus(ModListStatus status, std::string const& message, std::optional<std::string> const& details = std::nullopt);
};

class ModsLayer : public CCLayer {
protected:
    CCNode* m_frame;
    std::vector<CCMenuItemSpriteExtra*> m_tabs;
    ModListSource* m_currentSource = nullptr;
    std::unordered_map<ModListSource*, Ref<ModList>> m_lists;

    bool init();

    void keyBackClicked() override;
    void onTab(CCObject* sender);

public:
    static ModsLayer* create();
    static ModsLayer* scene();

    void onBack(CCObject*);
    void onRefreshList(CCObject*);

    void gotoTab(ModListSource* src = nullptr);
};
