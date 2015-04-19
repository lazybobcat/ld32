#include <datatables.h>
#include <resources/particle.h>


std::vector<ParticleData> initializeParticleData()
{
    std::vector<ParticleData> data(Particle::ParticleCount);

    data[Particle::Default].color = sf::Color(255, 255, 50);
    data[Particle::Default].lifetime = sf::seconds(0.6f);

    return data;
}

std::vector<std::vector<Phase>> initializeBossPhases(World &world, Boss &boss)
{
    std::vector<std::vector<Phase>> data(Boss::TypeCount);

    // Welcome to one of the most un-readable code I've ever made :

    // Boss1 Phase1
    /*Phase b1p1(world, boss, sf::seconds(7.f));
    b1p1.addSkill(sf::seconds(0.f), [&](Boss& b, sf::Time){
        PlayerEntity* player = world.player();
        world.shakeCameraFor(0.3f);
        if((b.getPosition().x-player->getPosition().x) > 0) {
            boss.move(Entity::Left);
        } else {
            boss.move(Entity::Right);
        }
    });
    data[Entity::Boss1].push_back(b1p1);
    // Boss1 Phase2
    Phase b1p2(world, boss, sf::seconds(5.f));
    b1p2.addSkill(sf::seconds(0.f), [&](Boss& b, sf::Time){
        b.knock();
    });
    data[Entity::Boss1].push_back(b1p2);
    // Boss1 Phase3
    Phase b1p3(world, boss, sf::seconds(2.f));
    b1p3.addSkill(sf::seconds(0.f), [&](Boss& b, sf::Time dt){
        b.setPosition(sf::Vector2f(b.getPosition().x, b.getPosition().y-500*dt.asSeconds()));
    });
    data[Entity::Boss1].push_back(b1p3);
    // Boss1 Phase4
    Phase b1p4(world, boss, sf::seconds(0.f));
    b1p4.addSkill(sf::seconds(0.f), [&](Boss& b, sf::Time dt){
        PlayerEntity* player = world.player();
        if(player->getPosition().x < 512) {
            b.setPosition(sf::Vector2f(-50.f, 512-105));
            for(unsigned int i = 0; i < 10; ++i)
            {
                world.addCreature(2, 50.f + i*32.f);
            }
            world.addCreature(1, 380.f);
        } else {
            b.setPosition(sf::Vector2f(1024+50.f, 512-105));
            for(unsigned int i = 0; i < 10; ++i)
            {
                world.addCreature(2, -50.f - i*32.f);
            }
            world.addCreature(1, -380.f);
        }
    });
    data[Entity::Boss1].push_back(b1p4);

    //////////////////////////////////////////////

    // Boss2 Phase2
    Phase b2p2(world, boss, sf::seconds(2.f));
    b2p2.addSkill(sf::seconds(0.90f), [&](Boss&, sf::Time){
    });
    data[Entity::Boss2].push_back(b2p2);
    // Boss2 Phase1
    Phase b2p1(world, boss, sf::seconds(1.2f));
    b2p1.addSkill(sf::seconds(0.5f), [&](Boss& b, sf::Time){
        PlayerEntity* player = world.player();
        if((b.getPosition().x-player->getPosition().x) > 0) {
            world.addCreature(2, 50.f);
        } else {
            world.addCreature(2, -50.f);
        }
    });
    data[Entity::Boss2].push_back(b2p1);
    // Boss2 Phase2
    data[Entity::Boss2].push_back(b2p2);
    data[Entity::Boss2].push_back(b2p1);
    data[Entity::Boss2].push_back(b2p2);
    data[Entity::Boss2].push_back(b2p1);
    // Boss1 Phase4
    Phase b2p3(world, boss, sf::seconds(0.f));
    b2p3.addSkill(sf::seconds(0.f), [&](Boss& b, sf::Time dt){
        PlayerEntity* player = world.player();
        if(b.getPosition().x < 512) {
            for(unsigned int i = 0; i < 5; ++i)
            {
                world.addCreature(2, 50.f + i*32.f);
            }
            world.addCreature(1, 380.f);
        } else {
            for(unsigned int i = 0; i < 5; ++i)
            {
                world.addCreature(2, -50.f - i*32.f);
            }
            world.addCreature(1, -380.f);
        }
    });
    data[Entity::Boss2].push_back(b2p3);
    // Boss2 Phase4
    Phase b2p4(world, boss, sf::seconds(4.f));
    b2p4.addSkill(sf::seconds(0.f), [&](Boss& b, sf::Time){
        PlayerEntity* player = world.player();
        if((b.getPosition().x-player->getPosition().x) > 0) {
            boss.move(Entity::Left);
        } else {
            boss.move(Entity::Right);
        }
    });
    data[Entity::Boss2].push_back(b2p4);
    // Boss2 Phase5
    Phase b2p5(world, boss, sf::seconds(2.f));
    b2p5.addSkill(sf::seconds(0.90f), [&](Boss& b, sf::Time){
        b.knock();
    });
    data[Entity::Boss2].push_back(b2p5);
    // Boss2 Phase6
    Phase b2p6(world, boss, sf::seconds(0.0f));
    b2p6.addSkill(sf::seconds(0.f), [&](Boss& b, sf::Time){
        PlayerEntity* player = world.player();
        if((b.getPosition().x-player->getPosition().x) > 0) {
            world.addCreature(3, 50.f);
        } else {
            world.addCreature(3, -50.f);
        }
    });
    data[Entity::Boss2].push_back(b2p6);
    // Boss2 Phase7
    Phase b2p7(world, boss, sf::seconds(4.1f));
    b2p7.addSkill(sf::seconds(0.f), [&](Boss& b, sf::Time){
        PlayerEntity* player = world.player();
        if((b.getPosition().x-player->getPosition().x) > 0) {
            boss.move(Entity::Right);
        } else {
            boss.move(Entity::Left);
        }
    });
    data[Entity::Boss2].push_back(b2p7);

    //////////////////////////////////////////////

    // Boss3 Phase1
    Phase b3p1(world, boss, sf::seconds(0.6f));
    b3p1.addSkill(sf::seconds(0.5f), [&](Boss& b, sf::Time){
        for(unsigned int i = 0; i < 6; ++i)
        {
            world.addCreature(3, 50.f + 150.f*i);
            world.addCreature(3, -50.f - 150.f*i);
        }
    });
    data[Entity::Boss3].push_back(b3p1);
    Phase b3p2(world, boss, sf::seconds(5.f));
    b3p2.addSkill(sf::seconds(0.90f), [&](Boss& b, sf::Time){b.knock();});
    data[Entity::Boss3].push_back(b3p2);
    // Boss1 Phase1
    Phase b3p3(world, boss, sf::seconds(5.f));
    b3p3.addSkill(sf::seconds(0.f), [&](Boss& b, sf::Time){
        PlayerEntity* player = world.player();
        world.shakeCameraFor(0.3f);
        if((b.getPosition().x-player->getPosition().x) > 0) {
            boss.move(Entity::Left);
        } else {
            boss.move(Entity::Right);
        }
    });
    data[Entity::Boss3].push_back(b3p3);
    data[Entity::Boss3].push_back(b3p1);
    // Boss1 Phase4
    Phase b3p4(world, boss, sf::seconds(3.f));
    b3p4.addSkill(sf::seconds(0.f), [&](Boss& b, sf::Time){
        b.knock();
    });
    data[Entity::Boss3].push_back(b3p4);
    // Boss3 Phase5
    Phase b3p5(world, boss, sf::seconds(2.f));
    b3p5.addSkill(sf::seconds(0.f), [&](Boss& b, sf::Time dt){
        b.setPosition(sf::Vector2f(b.getPosition().x, b.getPosition().y-500*dt.asSeconds()));
    });
    data[Entity::Boss3].push_back(b3p5);
    data[Entity::Boss3].push_back(b3p2);
    // Boss3 Phase6
    Phase b3p6(world, boss, sf::seconds(0.0f));
    b3p6.addSkill(sf::seconds(0.f), [&](Boss& b, sf::Time dt){
        PlayerEntity* player = world.player();
        b.setPosition(sf::Vector2f(player->getPosition().x, -160));
        world.addCreature(1, 50);
    });
    data[Entity::Boss3].push_back(b3p6);
    Phase b3p6bis(world, boss, sf::seconds(1.5f));
    b3p6bis.addSkill(sf::seconds(0.90f), [&](Boss& b, sf::Time){b.knock();});
    data[Entity::Boss3].push_back(b3p6bis);
    // Boss3 Phase7
    Phase b3p7(world, boss, sf::seconds(1.3f));
    b3p7.addSkill(sf::seconds(0.f), [&](Boss& b, sf::Time dt){
        if(b.getPosition().y < 512-105)
        {
            b.setPosition(sf::Vector2f(b.getPosition().x, b.getPosition().y+500*dt.asSeconds()));
        }
        else
        {
            world.shakeCamera();
            b.playLocalSound(world.getCommandQueue(), Sounds::BossCrush);
        }
    });
    data[Entity::Boss3].push_back(b3p7);
    data[Entity::Boss3].push_back(b3p1);
    // Boss3 Phase8
    data[Entity::Boss3].push_back(b3p5);
    data[Entity::Boss3].push_back(b3p6);
    data[Entity::Boss3].push_back(b3p6bis);
    data[Entity::Boss3].push_back(b3p7);
    data[Entity::Boss3].push_back(b3p1);
    data[Entity::Boss3].push_back(b3p5);
    // Boss3 Phase9
    Phase b3p9(world, boss, sf::seconds(1.5f));
    b3p9.addSkill(sf::seconds(0.f), [&](Boss& b, sf::Time dt){
        b.setPosition(1024+160, 512-105);
    });
    data[Entity::Boss3].push_back(b3p9);*/

    return data;
}
