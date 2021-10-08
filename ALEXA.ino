
void firstLightChanged(uint8_t brightness)
{

  if (brightness == 255)
    {
      digitalWrite(1,HIGH);
    }
  else
  {
    digitalWrite(1,LOW);
  }
}
