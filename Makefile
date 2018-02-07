.PHONY: clean All

All:
	@echo "----------Building project:[ DetectLED - Debug ]----------"
	@cd "DetectLED" && "$(MAKE)" -f  "DetectLED.mk"
clean:
	@echo "----------Cleaning project:[ DetectLED - Debug ]----------"
	@cd "DetectLED" && "$(MAKE)" -f  "DetectLED.mk" clean
