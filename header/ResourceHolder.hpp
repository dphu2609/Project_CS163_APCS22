#ifndef RESOURCEHOLDER_HPP
#define RESOURCEHOLDER_HPP

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>


namespace Textures {
    enum ID {
        PlayButton,
        PlayButtonHovered,
        PauseButton,
        PauseButtonHovered,
        NextButton,
        NextButtonHovered,
        PrevButton,
        PrevButtonHovered,
        ReplayButton,
        ReplayButtonHovered
    };
};

namespace Fonts {
    enum ID {
        ComfortaaRegular,
        FiraSansRegular,
        GreatVibesRegular,
        TiltWarpRegular,
        RobotoRegular,
        RobotoItalic,
        RobotoBold,
        FiraMonoRegular
    };
};

template <typename Resource, typename Identifier>
class ResourceHolder {
public:
    void load(Identifier id, const std::string& filename);
    Resource& operator [] (Identifier id);
    const Resource& operator [] (Identifier id) const;
private:
    std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

#include "ResourceHolder.inl"
#endif