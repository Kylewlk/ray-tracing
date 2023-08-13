//
// Created by wlk12 on 2023/8/13.
//

#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() = default;
    explicit hittable_list(shared_ptr<hittable> object) { add(std::move(object)); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object) {
        objects.push_back(std::move(object));
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;

        for (const auto& object : objects) {
            if (object->hit(r, ray_t, temp_rec)) {
                hit_anything = true;
                ray_t.max = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};
