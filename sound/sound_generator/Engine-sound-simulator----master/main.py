import controls
import engine_factory
from audio_device import AudioDevice
import random
import time 
import pygame

# Initialiser pygame et la manette
pygame.init()
pygame.joystick.init()

# S'assurer qu'une manette est connectée
if pygame.joystick.get_count() == 0:
    raise Exception("Aucune manette détectée")
    
# Initialiser la manette Xbox
joystick = pygame.joystick.Joystick(0)
joystick.init()



# engine = engine_factory.v_four_90_deg()
#engine = engine_factory.w_16()
#engine = engine_factory.v_8_LS()
# engine = engine_factory.inline_5_crossplane()
# engine = engine_factory.inline_6()
engine = engine_factory.boxer_4_crossplane_custom([1, 1, 0, 0])  # (rando := random.randrange(360)))
# engine = engine_factory.inline_4_1_spark_plug_disconnected()
# engine = engine_factory.inline_4()
# engine = engine_factory.boxer_4_half()
# engine = engine_factory.random()
#engine = engine_factory.fake_rotary_2rotor()
#engine = engine_factory.V_12()

audio_device = AudioDevice()
stream = audio_device.play_stream(engine.gen_audio)

print('\nEngine is running...')
# print(rando)
cycle_duration = 0.05
# Paramètres pour simuler le comportement de l'accélérateur
acceleration_rate = 2000  # vitesse d'accélération (RPM/s)
deceleration_rate = 1000  # vitesse de décélération (RPM/s)

oscillation_enabled = False  # Active l'oscillation
oscillation_lower_rpm = engine.limiter_rpm - 1500  # RPM minimum de l'oscillation
oscillation_upper_rpm = engine.limiter_rpm +200  # RPM maximum de l'oscillation
oscillation_direction = -1  # -1 pour descendre, 1 pour monter

try:
    while True:
        # Rafraîchir les événements pygame
        pygame.event.pump()

        # Lire la valeur de la gâchette droite (Right Trigger)
        trigger_value = joystick.get_axis(5)  # Axe 5 = gâchette droite sur manette Xbox

        # La gâchette renvoie une valeur de -1 (aucune pression) à 1 (pression maximale)
        throttle_value = (trigger_value + 1) / 2  # Normaliser entre 0 et 1

        # Si l'oscillation est activée
        if oscillation_enabled:
            if engine._rpm >= oscillation_upper_rpm:
                oscillation_direction = -1  # Commence à diminuer le RPM
            elif engine._rpm <= oscillation_lower_rpm:
                oscillation_direction = 1  # Remonte le RPM

            # Appliquer la direction d'oscillation
            engine._rpm += oscillation_direction * (acceleration_rate * cycle_duration)

        else:
            # Mapper la valeur de la gâchette sur les RPM du moteur (idle_rpm à limiter_rpm)
            desired_rpm = engine.idle_rpm + throttle_value * (engine.limiter_rpm - engine.idle_rpm)

            # Simuler une accélération fluide lorsque la gâchette est pressée
            if engine._rpm < desired_rpm:
                engine._rpm += min(acceleration_rate * cycle_duration, desired_rpm - engine._rpm)
            elif engine._rpm > desired_rpm:
                engine._rpm -= min(deceleration_rate * cycle_duration, engine._rpm - desired_rpm)

        # Activer l'oscillation quand on est proche du RPM max
        if engine._rpm >= engine.limiter_rpm - 200:
            oscillation_enabled = True
        else:
            oscillation_enabled = False

        print(f"Gâchette: {throttle_value:.2f} - RPM: {engine._rpm:.0f}", end='\r')

        # Attendre un peu avant la prochaine itération pour simuler un comportement temps réel
        time.sleep(cycle_duration)

except KeyboardInterrupt:
    pass

print('Exiting...')
stream.close()
audio_device.close()
pygame.quit()
