#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "Transformations.h"

enum class LightType {
    POINT = 0,
    DIRECTIONAL = 1,
    SPOTLIGHT = 2
};

class Light {
public:
    LightType type = LightType::POINT;
    glm::vec3 color{ 1.0f, 1.0f, 1.0f };
    glm::vec3 direction{ 0.0f, -1.0f, 0.0f };

    // Útlum pro bodová světla a reflektory
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    // Reflektor: kosinus mezního úhlu (default 0.99 odpovídá cca 8.1 stupňům)
    float spotCutoff = 0.99f;

    // Stav zapnuto / vypnuto
    bool enabled = true;

    // Statické světlo (pozice, barva)
    Light(const glm::vec3& staticPos = glm::vec3(0.0f), const glm::vec3& col = glm::vec3(1.0f));

    // Dynamické světlo napojené na transformaci (transformace, barva)
    Light(std::shared_ptr<ITransformation> transform, const glm::vec3& col = glm::vec3(1.0f));

    // Vrací aktuální pozici světla ve world-space
    glm::vec3 getPosition() const;

    void setStaticPosition(const glm::vec3& pos);
    void attachTo(std::shared_ptr<ITransformation> transform);
    std::shared_ptr<ITransformation> getAttachedTransform() const { return attachedTransform; }

private:
    glm::vec3 staticPosition{ 0.0f, 0.0f, 0.0f };
    std::shared_ptr<ITransformation> attachedTransform = nullptr;
};
