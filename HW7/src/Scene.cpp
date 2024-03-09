//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum){
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
        const Ray &ray,
        const std::vector<Object*> &objects,
        float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear) {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }


    return (*hitObject != nullptr);
}

// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray &ray, int depth) const
{
    // TO DO Implement Path Tracing Algorithm here
    Vector3f color(0.f);
    if(depth > this->maxDepth) return color;

    Intersection inter;
    inter=this->intersect(ray);
    if(!inter.happened) return color;

    if(inter.m->hasEmission()) color+=inter.m->getEmission();

    Vector3f point=inter.coords;
    Intersection lightSample;
    float lightPdf;
    this->sampleLight(lightSample, lightPdf);
    Ray directRay(point, normalize(lightSample.coords - point));
    Intersection middle=this->intersect(directRay);
    if(middle.happened && middle.m->hasEmission())
    // if(middle.distance*middle.distance>dotProduct(lightSample.coords - point,lightSample.coords - point)-EPSILON)//检查与光源之间是否有物体遮挡
    {
        Vector3f f_r=inter.m->eval(ray.direction, directRay.direction, inter.normal);
        double cos_theta=dotProduct(directRay.direction, inter.normal);
        Vector3f L_r=lightSample.emit;
        double cos_theta_light=dotProduct(-directRay.direction, lightSample.normal);
        double distance_square= dotProduct(lightSample.coords - point, lightSample.coords - point);
        color+=L_r*cos_theta*f_r*cos_theta_light/(distance_square*lightPdf);
    }

    if(get_random_float()<this->RussianRoulette)//RussianRoulette概率继续反射
    {
        Vector3f indirectRayDir=normalize(inter.m->sample(ray.direction, inter.normal));
        Ray indirectRay(point, indirectRayDir);
        Intersection indirectInter=this->intersect(indirectRay);
        if(indirectInter.happened && !indirectInter.m->hasEmission())//与非光源物体相交
        {
            Vector3f L_r=castRay(indirectRay, depth+1);
            float pdf=inter.m->pdf(ray.direction, indirectRayDir, inter.normal);
            Vector3f f_r=inter.m->eval(ray.direction, indirectRayDir, inter.normal);
            float cos_theta=dotProduct(indirectRayDir, inter.normal);
            color+=L_r*f_r*cos_theta/(pdf*this->RussianRoulette);
        }
    }

    return color;
    
}