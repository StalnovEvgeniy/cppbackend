#ifndef MODEL_GAME_H
#define MODEL_GAME_H

#pragma once

#include <vector>
#include "util/tagged.h"
#include "map.h"

#include "gamesession.h"

namespace model {

class Game {
public:
    using Maps = std::vector<Map>;
    using Sessions = std::vector<GameSession>;

    void AddMap(Map map);

    const Maps &GetMaps() const noexcept;

    const Map *FindMap(const Map::Id& id) const noexcept;

    void AddGameSession(GameSession session);
    const GameSession *FindGameSessionBy(const Map::Id& id) const noexcept;

private:
    using MapIdHasher = util::TaggedHasher<Map::Id>;
    using MapIdToIndex = std::unordered_map<Map::Id, size_t, MapIdHasher>;


    Maps maps_;
    MapIdToIndex map_id_to_index_;

    using MapIdToSessionIndex = std::unordered_map<Map::Id, size_t, MapIdHasher>;

    Sessions sessions_;
    MapIdToSessionIndex map_id_to_session_index_;

};

}  // namespace model

#endif // MODEL_GAME_H
